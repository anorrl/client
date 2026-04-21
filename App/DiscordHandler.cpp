#include "stdafx.h"

#include "DiscordHandler.h"

namespace ARL {

	void DiscordHandler::SetDetails(std::string state, std::string details) {
        DiscordRichPresence discordPresence;
        memset(&discordPresence, 0, sizeof(discordPresence));
        discordPresence.state = state.c_str();
        discordPresence.details = details.c_str();
        /*discordPresence.endTimestamp = time(0) + 5 * 60;
        discordPresence.largeImageKey = "canary-large";
        discordPresence.smallImageKey = "ptb-small";*/
        discordPresence.instance = 0;
        Discord_UpdatePresence(&discordPresence);
	}

    void DiscordHandler::Initialise(const char* applicationID) {
		Discord_Initialize(applicationID, NULL, 1, NULL);
    }

    void DiscordHandler::Callback() {
        Discord_RunCallbacks();
    }               
}