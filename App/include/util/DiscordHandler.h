#pragma once

#include <string>

#if !defined(ARL_ACC_SECURITY) && !defined(__linux__)
#include "discord_rpc.h"
#endif

namespace ARL {
	class DiscordHandler {
		static bool initialized;
		static std::string state;
		static std::string details;
	public:
		static void SetDetails(std::string details);
		static void SetState(std::string state);
		static void Initialise(const char* applicationID);
		static void UpdateActivity();
		static void Shutdown();
	};
}
