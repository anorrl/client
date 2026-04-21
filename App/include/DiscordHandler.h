#pragma once

#include <string>

#include "discord_rpc.h"

namespace ARL {
	class DiscordHandler {
	public:
		static void SetDetails(std::string state, std::string details);
		static void Callback();
		static void Initialise(const char* applicationID);
	};
}