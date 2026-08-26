#include "stdafx.h"

#include "V8DataModel/DiscordRPCService.h"

#ifndef ARL_ACC_SECURITY
#include "DiscordHandler.h"
#endif
namespace ARL
{
	const char* const sDiscordRPCService = "DiscordRPCService";

	static Reflection::BoundFuncDesc<DiscordRPCService, void(std::string)> func_setState(&DiscordRPCService::setState, "SetState", "state", Security::None);
	static Reflection::BoundFuncDesc<DiscordRPCService,     std::string()> func_getState(&DiscordRPCService::getState, "GetState", Security::None);

	static Reflection::BoundFuncDesc<DiscordRPCService, void(std::string)> func_setDetails(&DiscordRPCService::setDetails, "SetDetails", "details", Security::None);
	static Reflection::BoundFuncDesc<DiscordRPCService,     std::string()> func_getDetails(&DiscordRPCService::getDetails, "GetDetails", Security::None);

	DiscordRPCService::DiscordRPCService()
		:Super()
	{
#ifndef ARL_ACC_SECURITY
#ifdef ARL_STUDIO_BUILD
		DiscordHandler::Initialise("1456345979635892400");
#else
		DiscordHandler::Initialise("1456740539390230609");
#endif
#endif
		this->setName(sDiscordRPCService);
	}

	void DiscordRPCService::setState(std::string value) {
		if (state != value) {
			state = value;
#ifndef ARL_ACC_SECURITY
			DiscordHandler::SetState(state);
#endif
		}
	}

	void DiscordRPCService::setDetails(std::string value) {
		if (details != value) {
			details = value;
#ifndef ARL_ACC_SECURITY
			DiscordHandler::SetDetails(details);
#endif
		}
	}
}
