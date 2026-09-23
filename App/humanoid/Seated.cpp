/* Copyright 2003-2005 ROBLOX Corporation, All Rights Reserved */
#include "stdafx.h"

#include "humanoid/Seated.h"
#include "humanoid/Humanoid.h"

namespace ARL {
namespace HUMAN {

const char* const sSeated = "Seated";

Seated::Seated(Humanoid* humanoid, StateType priorState) : Named<HumanoidState, sSeated>(humanoid, priorState)
{
	setCanThrottleState(true);
}

// note - constructor always sets to false

Seated::~Seated()
{
	getHumanoid()->setSeatPart(NULL);
	getHumanoid()->setSit(false);
	getHumanoid()->doneSittingSignal();
}

///////////////////////////////////////////////

const char* const sPlatformStanding = "PlatformStanding";

PlatformStanding::PlatformStanding(Humanoid* humanoid, StateType priorState) : Named<HumanoidState, sPlatformStanding>(humanoid, priorState)
{
	setCanThrottleState(true);
}

// note - constructor always sets to false

PlatformStanding::~PlatformStanding()
{
	getHumanoid()->setPlatformStanding(false);			
	getHumanoid()->donePlatformStandingSignal();
}




} // HUMAN
}	// namespace
