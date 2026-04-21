#include "network/WebChatFilter.h"

#include <algorithm>

#include "RobloxServicesTools.h"

#include "rbx/rbxTime.h"
#include "v8datamodel/Stats.h"
#include "V8DataModel/DataModel.h"
#include "util/Guid.h"
#include "util/Http.h"
#include "util/HttpAsync.h"
#include "util/standardout.h"
#include "reflection/Type.h"
#include "util/Statistics.h"
#include "util/RobloxGoogleAnalytics.h"

#include <boost/lexical_cast.hpp>
#include <boost/thread/once.hpp>
#include <rapidjson/document.h>

DYNAMIC_LOGVARIABLE(WebChatFiltering, 0)

DYNAMIC_FASTINTVARIABLE(WebChatFilterHttpTimeoutSeconds, 60)
DYNAMIC_FASTFLAGVARIABLE(UseComSiftUpdatedWebChatFilterParamsAndHeader, true)
DYNAMIC_FASTFLAGVARIABLE(ConstructModerationFilterTextParamsAndHeadersUseLegacyFilterParams, true)

using namespace ARL;
using namespace ARL::Network;

namespace
{
static Timer<ARL::Time::Precise> lastFailureTime;

void initFailureTimer()
{
	lastFailureTime.reset();
}

inline void logContent(const std::string &varName, const std::string &msg)
{
	if (DFLog::WebChatFiltering)
	{
		std::stringstream ss;
		ss << varName << " (" << msg.length() << " bytes) " << msg;
		FASTLOGS(DFLog::WebChatFiltering, "%s", ss.str().c_str());
	}
}

inline void logResponseData(
	const std::string& unfiltered,
	const ChatFilter::Result& result,
	const ARL::Time::Interval& webChatFilterResponseDelta)
{
    FASTLOG1F(DFLog::WebChatFiltering, "ResponseLatencyMillis: %f", webChatFilterResponseDelta.msec());
    logContent("FilteredContent (blacklist)", result.blacklistFilteredMessage);
    logContent("FilteredContent (whitelist)", result.whitelistFilteredMessage);
}
} // namespace

void filterMessageHelper(
	const std::string message,
	const ARL::Network::ChatFilter::FilteredChatMessageCallback filteredCallback,
	shared_ptr<ARL::Network::Player> playerFilter)
{
	logContent("Unfiltered", message);

	{
		static boost::once_flag flag = BOOST_ONCE_INIT;
		boost::call_once(&initFailureTimer, flag);
	}

	std::stringstream params;
	ARL::HttpAux::AdditionalHeaders headers;
	if (DFFlag::UseComSiftUpdatedWebChatFilterParamsAndHeader)
	{
		DataModel* dataModel = DataModel::get(playerFilter.get());

		ConstructModerationFilterTextParamsAndHeaders(
			message,
			playerFilter->getUserID(), 
			dataModel->getPlaceID(), 
			dataModel->getGameInstanceID(), 
			/*out*/ params, 
			/*out*/ headers);
	}
	else
	{
		params << "filters=" << kWebChatWhiteListPolicy;
		params << "&filters=" << kWebChatBlackListPolicy;
		params << "&text=" << Http::urlEncode(message);
	}


	ARL::Timer<ARL::Time::Precise> timer;

	std::string response;
	Time::Interval responseDelta = timer.delta();
	std::string failureReason;

	ChatFilter::Result result;
	try
	{
        Http http(GetWebChatFilterURL(GetBaseURL()));
        http.setResponseTimeout(DFInt::WebChatFilterHttpTimeoutSeconds * 1000);
		if (DFFlag::UseComSiftUpdatedWebChatFilterParamsAndHeader)
		{
			http.applyAdditionalHeaders(headers);
		}
        http.post(params, Http::kContentTypeUrlEncoded, false, response);

        using namespace rapidjson;
        Document doc;
        doc.Parse<kParseDefaultFlags>(response.c_str());

        ARLASSERT(doc.HasMember("data"));
        Value& data = doc["data"];
        ARLASSERT(data.HasMember(kWebChatWhiteListPolicy.c_str()));
        ARLASSERT(data.HasMember(kWebChatBlackListPolicy.c_str()));

        result.whitelistFilteredMessage = data[kWebChatWhiteListPolicy.c_str()].GetString();
        result.blacklistFilteredMessage = data[kWebChatBlackListPolicy.c_str()].GetString();

        filteredCallback(result);
	}
	catch (const std::exception& e)
	{
		FASTLOGS(DFLog::WebChatFiltering, "Web chat failed: %s", e.what());
        failureReason = e.what();
        RobloxGoogleAnalytics::trackEvent(GA_CATEGORY_GAME, "ChatFailure", failureReason.c_str());
	}

	logResponseData(message, result, responseDelta);
}

void WebChatFilter::filterMessage(
	shared_ptr<ARL::Network::Player> sender,
	shared_ptr<ARL::Instance> receiver,
	const std::string& message,
	const ARL::Network::ChatFilter::FilteredChatMessageCallback filteredCallback)
{
    if (filterMessageBase(sender, receiver, message, filteredCallback))
    {
        return;
    }

	ARLASSERT(sender);
	boost::function0<void> f = boost::bind(&filterMessageHelper, message, filteredCallback, sender);
	boost::function0<void> g = boost::bind(&StandardOut::print_exception, f, ARL::MESSAGE_ERROR, false);
	boost::thread(thread_wrapper(g, "rbx_webChatFilterHttpPost"));
}

void ARL::Network::ConstructModerationFilterTextParamsAndHeaders(
	std::string text,
	int userID,
	int placeID,
	std::string gameInstanceID,

	std::stringstream &outParams,
	ARL::HttpAux::AdditionalHeaders &outHeaders
)
{
    outParams << "text=" << Http::urlEncode(text);
    if (DFFlag::ConstructModerationFilterTextParamsAndHeadersUseLegacyFilterParams) {
        outParams << "&filters=" << kWebChatWhiteListPolicy;
        outParams << "&filters=" << kWebChatBlackListPolicy;
    }
	outParams << "&userId=" << userID;

    outHeaders["placeId"] = boost::lexical_cast<std::string>(placeID);
	outHeaders["gameInstanceID"] = gameInstanceID;
}

