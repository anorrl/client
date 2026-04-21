#pragma once

#include "network/ChatFilter.h"
#include "Network/Player.h"
#include "Util/Http.h"
#include "util/HttpAux.h"

#include <boost/shared_ptr.hpp>

static const std::string kWebChatWhiteListPolicy = "white";
static const std::string kWebChatBlackListPolicy = "black";

namespace ARL
{
namespace Network
{

class WebChatFilter : public ARL::Network::ChatFilter
{
public:
	/*override*/
	virtual void filterMessage(
		shared_ptr<ARL::Network::Player> sourcePlayer,
		shared_ptr<ARL::Instance> receiver,
		const std::string& message,
		const ARL::Network::ChatFilter::FilteredChatMessageCallback callback);
}; // class WebChatFilter

void ConstructModerationFilterTextParamsAndHeaders(
	std::string text,
	int userID,
	int placeID,
	std::string gameInstanceID,

	std::stringstream &outParams,
	ARL::HttpAux::AdditionalHeaders &outHeaders
);

} // namespace Network
} // namespace ARL

