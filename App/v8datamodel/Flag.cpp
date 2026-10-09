/* Copyright 2003-2007 ROBLOX Corporation, All Rights Reserved */
#include "stdafx.h"

#include "v8datamodel/Flag.h"
#include "v8datamodel/FlagStand.h"
#include "v8datamodel/Workspace.h"
#include "v8tree/Instance.h"
#include "network/Players.h"
#include "network/Player.h"
#include "humanoid/Humanoid.h"
#include "v8world/Primitive.h"
#include "v8world/RigidJoint.h"
#include "util/RunStateOwner.h"
#include "v8datamodel/TimerService.h"

namespace ARL {

using namespace ARL::Network;

const char* const sFlag = "Flag";

REFLECTION_BEGIN();
static const Reflection::PropDescriptor<Flag, BrickColor> prop_Color("TeamColor", category_Data, &Flag::getTeamColor, &Flag::setTeamColor);
REFLECTION_END();

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// FRONTEND AND BACKEND

Flag::Flag() 
: DescribedCreatable<Flag, Tool, sFlag>(),
	flagTouched(FLog::TouchedSignal)
{
	setName(sFlag);
}


Flag::~Flag()
{
}


BrickColor Flag::getTeamColor() const 
{
	return teamColor; 
}

void Flag::setTeamColor(BrickColor color) 
{
	this->teamColor = color;
	raisePropertyChanged(prop_Color);
}

void Flag::onServiceProvider(ServiceProvider* oldProvider, ServiceProvider* newProvider)
{
	if (oldProvider == NULL) {
		if (Network::Players::backendProcessing(this)) {
			if (PartInstance *part = this->getHandle()) {
				flagTouched = part->onDemandWrite()->touchedSignal.connect(boost::bind(&Flag::onEvent_flagTouched, this, _1));
				FASTLOG3(FLog::TouchedSignal, "Connecting Flag to touched signal, instance: %p, part: %p, part signal: %p", 
					this, part, &(part->onDemandWrite()->touchedSignal));
			}
		}
	}

	Super::onServiceProvider(oldProvider, newProvider);

	if (newProvider == NULL) {
		flagTouched.disconnect();
	}
}


bool Flag::canBePickedUpByPlayer(Player *p)
{
	if (p->getNeutral()) return false; // neutral players can't interfere

	if (p->getTeamColor() == this->getTeamColor()) return false;

	return true;
}


void Flag::onEvent_flagTouched(shared_ptr<Instance> other)
{
	if (Network::Players::backendProcessing(this))
	{
		Instance* touchingCharacter = other->getParent();

		Humanoid* humanoid = Humanoid::modelIsCharacter(touchingCharacter);
		if (!humanoid) {
			return;
		}

		Player *p = Players::getPlayerFromCharacter(touchingCharacter);
		if (!p)
			return;

		if (p->getNeutral()) 
			return; // neutral players can't affect the flag

		if (p->getTeamColor() == this->getTeamColor())
		{
			// If the player touching a flag is the same team as the flag, AND the flag is not in a flag stand, return it
			// to the nearest flag stand.
			FlagStandService *fss = ServiceProvider::create<FlagStandService>(this);
			if (!fss->FindStandWithFlag(this)) 
			{
				// this flag is not in a flagstand
				fss->affixFlagToRandomEmptyStand(this);
			}
		}
	}
}

FlagStand *Flag::getJoinedStand()
{
	if (PartInstance* handle = this->getHandle())
	{
		Primitive *handlePrim = handle->getPartPrimitive();
		RigidJoint *r = handlePrim->getFirstRigid();
		while(r != NULL)
		{
			PartInstance *p = PartInstance::fromPrimitive(r->otherPrimitive(handlePrim));
			FlagStand *fs = Instance::fastDynamicCast<FlagStand>(p); // ASSUME: A part's parent will be the flag, if this is a flag.
			if (fs) return fs;
			r = handlePrim->getNextRigid(r);
		}
	}
	return NULL;
}


} // namespace
