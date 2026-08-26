#include "stdafx.h"

#include "DiscordHandler.h"

namespace ARL {

	bool DiscordHandler::initialized = false;
	std::string DiscordHandler::state = std::string("[[ state here ]]");
	std::string DiscordHandler::details = std::string("[[ details here ]]");

	void DiscordHandler::SetDetails(std::string value) {
		if (!initialized)
			return;

		details = value;

		UpdateActivity();
	}

	void DiscordHandler::SetState(std::string value) {
		if (!initialized)
			return;

		state = value;

		UpdateActivity();
	}

	void DiscordHandler::Initialise(const char* applicationID)
	{
		if (initialized)
			return;

#ifndef ARL_ACC_SECURITY
		DiscordEventHandlers handlers;
		memset(&handlers, 0, sizeof(handlers));
		
		Discord_Initialize(applicationID, &handlers, 0, NULL);
#endif
		// This is really unsafe but works anyway
		initialized = true;
		UpdateActivity();
	}

	void DiscordHandler::UpdateActivity()
	{
		if (!initialized)
			return;

#ifndef ARL_ACC_SECURITY
		DiscordRichPresence discordPresence;
		memset(&discordPresence, 0, sizeof(discordPresence));
		discordPresence.state = state.c_str();
		discordPresence.details = details.c_str();
		discordPresence.startTimestamp = std::time(0);
		discordPresence.instance = 1;
		Discord_UpdatePresence(&discordPresence);
#endif

		/*discordPresence.endTimestamp = time(0) + 5 * 60;
		discordPresence.largeImageKey = "canary-large";
		discordPresence.smallImageKey = "ptb-small";*/
		
	}

	void DiscordHandler::Shutdown()
	{
		if (!initialized)
			return;
#ifndef ARL_ACC_SECURITY
		Discord_ClearPresence();
		Discord_Shutdown();
#endif
		initialized = false;
	}
}