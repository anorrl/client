#include "stdafx.h"

#include "util/Analytics.h"
#include "util/ThreadPool.h"
#include "util/Http.h"
#include "util/Statistics.h"

#include "v8datamodel/GameBasicSettings.h"
#include "v8datamodel/DebugSettings.h"

#include "ANORRLServicesTools.h"

#include <boost/algorithm/string.hpp>

using namespace ARL;

DYNAMIC_LOGVARIABLE(AnalyticsLog, 0)

// google analytics
FASTFLAG(DebugAnalyticsForceLotteryWin)
DYNAMIC_FASTSTRING(ANORRLAnalyticsURL)
DYNAMIC_FASTFLAG(ANORRLAnalyticsTrackingEnabled)
DYNAMIC_FASTFLAG(DebugAnalyticsSendUserId)
FASTFLAG(SendStudioEventsWithStudioSID)
FASTFLAG(UseBuildGenericGameUrl)

// influx
DYNAMIC_FASTSTRING(HttpInfluxURL)
DYNAMIC_FASTSTRING(HttpInfluxDatabase)
DYNAMIC_FASTSTRING(HttpInfluxUser)
DYNAMIC_FASTSTRING(HttpInfluxPassword)
DYNAMIC_FASTFLAGVARIABLE(InfluxDb09Enabled, false)

namespace { // http utils

	const int kNumberThreadPoolThreads = 16;
	ThreadPool defaultThreadPool(kNumberThreadPoolThreads, BaseThreadPool::NoAction, 500);

	void httpHandler(std::string url, std::string params, std::string optionalContentType, bool isGet, bool externalRequest)
	{
		std::istringstream sparams(params);
		std::string response;

		std::string fullUrl = isGet ? format("%s?%s", url.c_str(), params.c_str()) : url;

		ARL::Http http(fullUrl);
		http.recordStatistics = false;
		http.doNotUseCachedResponse = true;
		try
		{
			if(isGet)
				http.get(response);
			else
				http.post(sparams, optionalContentType.empty() ? Http::kContentTypeUrlEncoded : optionalContentType, params.size() > 256, response, externalRequest);
		}
		catch (const ARL::base_exception &ex)
		{
			FASTLOGS(DFLog::AnalyticsLog, "Exception in analytics httpHandler: %s", ex.what());
		}
	}

	void postOrGet(ThreadPool& threadPool, const std::string& url, const std::string& data, const std::string& optionalContentType, bool blocking, bool isGet, bool externalRequest)
	{
		if (blocking)
		{
			httpHandler(url, data, optionalContentType, isGet, externalRequest);
		}
		else
		{
			bool didSchedule = threadPool.schedule(boost::bind(&httpHandler, url, data, optionalContentType, isGet, externalRequest));
			ARLASSERT(didSchedule);
			if (!didSchedule)
			{
				FASTLOGS(DFLog::AnalyticsLog, "Analytics task was not scheduled in the threadpool: %s", data.c_str());
			}
		}
	}
}

namespace ARL {
namespace Analytics {

	static std::string appVersion;
	static std::string reporter;
	static std::string location;
	static std::string userId;
	static std::string placeId;

	std::string sanitizeParam(const std::string& p)
	{
		std::string result = p;
		result.erase(std::remove_if(result.begin(), result.end(), boost::is_any_of(", ")), result.end());
		return result;
	}

	void setUserId(int id)
	{
		userId = ARL::format("%d", id);
	}

	void setPlaceId(int id)
	{
		placeId = ARL::format("%d", id);
	}

	void setAppVersion(const std::string& version)
	{
		appVersion = sanitizeParam(version);
	}

	void setLocation(const std::string& loc)
	{
		location = sanitizeParam(loc);
	}

	void setReporter(const std::string& rep)
	{
		reporter = sanitizeParam(rep);
	}

namespace EphemeralCounter
{
	const std::string countersApiKey =  "76E5A40C-3AE1-4028-9F10-7C62520BD94F";
	ThreadPool countersThreadPool(kNumberThreadPoolThreads, BaseThreadPool::NoAction, 500);

	void reportStats(const std::string& category, float value, bool blocking)
	{
		std::string baseUrl = ::GetBaseURL();
		if (baseUrl[baseUrl.size() - 1] != '/')
			baseUrl += '/';

		std::string valueStr = ARL::format("%f", value);
        std::string url;
		if (FFlag::UseBuildGenericGameUrl)
		{
			url = BuildGenericGameUrl(baseUrl, ARL::format("game/report-stats?name=%s&value=%s", Http::urlEncode(category).c_str(), Http::urlEncode(valueStr).c_str()));
		}
		else
		{
			url = ARL::format("%sgame/report-stats?name=%s&value=%s", baseUrl.c_str(), Http::urlEncode(category).c_str(), Http::urlEncode(valueStr).c_str());
		}

		// post
		postOrGet(countersThreadPool, url, "", Http::kContentTypeUrlEncoded, blocking, false, false);
	}

	void reportCountersCSV(const std::string& counterNamesCSV, bool blocking)
	{
		std::string counterUrl = GetCountersMultiIncrementUrl(::GetBaseURL(), countersApiKey);
		std::string data = "counterNamesCsv=" + counterNamesCSV;

		// post
		postOrGet(countersThreadPool, counterUrl, data, Http::kContentTypeUrlEncoded, blocking, false, false);
	}

	void reportCounter(const std::string& counterName, int amount, bool blocking)
	{
		std::string counterUrl = GetCountersUrl(::GetBaseURL(), countersApiKey);
		std::string url = ARL::format("%s&counterName=%s&amount=%d", counterUrl.c_str(), counterName.c_str(), amount);

		// post
		postOrGet(countersThreadPool, url, "", Http::kContentTypeUrlEncoded, blocking, false, false);
	}

} // namespace EphemeralCounter

namespace InfluxDb {

	int r = 10000; // throttle 

	std::size_t hash_value(const Point& p)
	{
		return boost::hash<std::string>()(p.name);
	}	

	void init()
	{
		r = rand() % 10000;
	}

	bool canSend(int throttleHundredthsPercentage)
	{
		return r < throttleHundredthsPercentage && !DFString::HttpInfluxURL.empty();
	}
    
    std::string escapeTag(std::string tag)
    {
        std::string s = tag;
        boost::replace_all(s, " ", "\\ ");
        boost::replace_all(s, ",", "\\,");
        return s;
    }

	using namespace ARL::Analytics::InfluxDb;

	void reportPointsV2(const std::string& resource, const boost::unordered_set<Point>& points, int throttleHundredthsPercentage, bool blocking, const std::string& userIdOverride)
	{
		if (!canSend(throttleHundredthsPercentage))
			return;

		ARLASSERT(!points.empty());

		// Pushes to InfluxDB
        std::string url = DFString::HttpInfluxURL + "/write?db=" + DFString::HttpInfluxDatabase + "&u=" + DFString::HttpInfluxUser + "&p=" + DFString::HttpInfluxPassword;

		// format: https://docs.influxdata.com/influxdb/v0.10/write_protocols/line/
		// <measurement>[,<tag-key>=<tag-value>...] <field-key>=<field-value>[,<field2-key>=<field2-value>...] [unix-nano-timestamp]

		// convert all guest IDs to -1
		std::string uid = (userIdOverride.empty() ? (!userId.empty() ? userId.c_str() : "0") : userIdOverride.c_str());
		if (atoi(uid.c_str()) < 0)
			uid = "-1";

        // tags
        std::stringstream ss;
		ss << ARL::format("%s,placeid=%s,userid=%s,reporter=%s,platform=%s,device=%s,appversion=%s,location=%s,osversion=%s ",
			resource.c_str(), 
			(!placeId.empty() ? placeId.c_str() : "0"), 
			uid.c_str(),
			escapeTag(reporter).c_str(),
			escapeTag(DebugSettings::singleton().osPlatform()).c_str(),
			escapeTag(DebugSettings::singleton().deviceName()).c_str(),
			escapeTag(appVersion).c_str(),
			escapeTag(location).c_str(),
			escapeTag(DebugSettings::singleton().osVer()).c_str());

		// fields
		boost::unordered_set<Point>::const_iterator it = points.begin();
		for (int index = 0; points.end() != it; ++it, ++index)
		{
            if (index > 0)
                ss << ',';
			ss << it->name << '=' << it->json;
		}

		postOrGet(defaultThreadPool, url, ss.str(),  "", blocking, false, true);
	}

	void reportPoints(const std::string& resource, const boost::unordered_set<Point>& points, int throttleHundredthsPercentage, bool blocking, const std::string& userIdOverride)
	{
		if (DFFlag::InfluxDb09Enabled)
		{
			reportPointsV2(resource, points, throttleHundredthsPercentage, blocking, userIdOverride);
			return;
		}

		if (!canSend(throttleHundredthsPercentage))
			return;

		ARLASSERT(!points.empty());

		// Pushes to InfluxDB.
		std::string url = DFString::HttpInfluxURL + "/db/" + DFString::HttpInfluxDatabase + "/series?u=" + DFString::HttpInfluxUser + "&p=" + Http::urlEncode(DFString::HttpInfluxPassword);

		std::string json = "[{";
		json += "\"name\":\"" + resource + "\",";
		json += "\"columns\":[\"placeid\",\"platform\",\"device\",\"osversion\",\"appversion\",\"userid\",\"reporter\",\"location\"";
		for (boost::unordered_set<Point>::const_iterator it = points.begin(); points.end() != it; ++it)
		{
			json += ",\"" + it->name + "\"";
		}
		json += "],";

		// two levels are here because we are actually sending a list of several points,
		// even though this function only handles one row of points at a time
		json += "\"points\":[[";
		json += (!placeId.empty() ? placeId : "0");
		json += ",\"" + DebugSettings::singleton().osPlatform() + "\"";
		json += ",\"" + DebugSettings::singleton().deviceName() + "\"";
		json += ",\"" + DebugSettings::singleton().osVer() + "\"";
		json += ",\"" + appVersion + "\"";

		if (!userIdOverride.empty())
			json += "," + userIdOverride;
		else
			json += "," + (!userId.empty() ? userId : "0");
		
		json += ",\"" + reporter + "\"";
		json += ",\"" + location + "\"";
		for (boost::unordered_set<Point>::const_iterator it = points.begin(); points.end() != it; ++it)
		{
			json += "," + it->json;
		}
		json += "]]";

		json += "}]";

		postOrGet(defaultThreadPool, url, json,  Http::kContentTypeApplicationJson, blocking, false, true);
	}

} // namespace InfluxDb
} // namespace ExternalAnalytics
} // namespace ARL
