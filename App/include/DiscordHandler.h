#pragma once

#include <string>

#ifndef ARL_ACC_SECURITY
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