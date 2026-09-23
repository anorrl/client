/* Copyright 2003-2013 ROBLOX Corporation, All Rights Reserved */
#include "stdafx.h"

#include "v8datamodel/AnimationController.h"
#include "v8datamodel/Workspace.h"
#include "v8datamodel/PartInstance.h"
#include "v8datamodel/Animator.h"
#include "v8world/World.h"
#include "v8tree/Service.h"
#include "v8kernel/Constants.h"
#include "network/Players.h"

namespace ARL {

const char* const sAnimationController = "AnimationController";

REFLECTION_BEGIN();
static Reflection::BoundFuncDesc<AnimationController, shared_ptr<const Reflection::ValueArray>()> desc_GetPlayingAnimationTracks(&AnimationController::getPlayingAnimationTracks, "GetPlayingAnimationTracks", Security::None);
static Reflection::BoundFuncDesc<AnimationController, shared_ptr<Instance>(shared_ptr<Instance>)> desc_LoadAnimation(&AnimationController::loadAnimation, "LoadAnimation","animation", Security::None);
static Reflection::EventDesc<AnimationController, void(shared_ptr<Instance>)> event_AnimationPlayed(&AnimationController::animationPlayedSignal, "AnimationPlayed", "animationTrack");
REFLECTION_END();

AnimationController::AnimationController() 
{
	setName("AnimationController");
	FASTLOG1(FLog::ISteppedLifetime, "AnimationController created - %p", this);
}

AnimationController::~AnimationController()
{
	FASTLOG1(FLog::ISteppedLifetime, "AnimationController destroyed - %p", this);
}


bool AnimationController::askSetParent(const Instance* instance) const
{
	return true;
}

void AnimationController::onStepped(const Stepped& event)
{
	if(animator)
	{
		animator->onStepped(event);
	}
}

Animator* AnimationController::getAnimator()
{
	if(!animator)
	{
		animator = shared_ptr<Animator>(Creatable<Instance>::create<ARL::Animator>(this));
        if (getParent() && getParent()->getClassNameStr() == "Model")
            animator->setParent(this);
	}
	return animator.get();
}

shared_ptr<const Reflection::ValueArray> AnimationController::getPlayingAnimationTracks()
{
	return getAnimator()->getPlayingAnimationTracks();
}

shared_ptr<Instance> AnimationController::loadAnimation(shared_ptr<Instance> instance)
{
	return getAnimator()->loadAnimation(instance);
}

void AnimationController::onServiceProvider(ServiceProvider* oldProvider, ServiceProvider* newProvider) 
{
	Super::onServiceProvider(oldProvider, newProvider);
	onServiceProviderIStepped(oldProvider, newProvider);
}

} // namespace
