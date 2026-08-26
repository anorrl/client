/**
 * MachineConfig.cpp
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#include "stdafx.h"
#include "MachineConfig.h"

// ANORRL Headers
#include "MachineConfiguration.h"

// ANORRL Studio Headers
#include "ANORRLSettings.h"

MachineConfig* MachineConfig::machineConfig = NULL;
////////////////////////////////////////////////////////////////////////////////////

void MachineConfig::PostMachineConfiguration(bool authSuccess)
{
	if(authSuccess)
	{
		disconnect(&AuthenticationHelper::Instance(), SIGNAL(authenticationDone(bool)), machineConfig, SLOT(PostMachineConfiguration(bool)));

        ANORRLSettings settings;
        int lastGfxMode = settings.contains("lastGFXMode") ? settings.value("lastGFXMode").toInt() : 0;

		ARL::postMachineConfiguration(ANORRLSettings::getBaseURL().toLocal8Bit().constData(), lastGfxMode);
	}
}
/////////////////////////////////////////////////////////////////////////////////
