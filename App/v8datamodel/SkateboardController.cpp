/* Copyright 2003-2005 ROBLOX Corporation, All Rights Reserved */
#include "stdafx.h"

#include "v8datamodel/SkateboardController.h"
#include "v8datamodel/UserController.h"
#include "v8datamodel/PVInstance.h"
#include "v8datamodel/Camera.h"
#include "v8datamodel/Workspace.h"
#include "v8datamodel/SkateboardPlatform.h"
#include "v8datamodel/UserInputService.h"
#include "network/Players.h"
#include "humanoid/Humanoid.h"
#include "V8tree/Service.h"
#include "util/UserInputBase.h"
#include "util/Math.h"
#include "util/NavKeys.h"


namespace ARL {

const char* const sSkateboardController = "SkateboardController";

REFLECTION_BEGIN();
static const Reflection::PropDescriptor<SkateboardController, float> propThrottle("Throttle", "Axes", &SkateboardController::getThrottle, NULL, Reflection::PropertyDescriptor::UI);
static const Reflection::PropDescriptor<SkateboardController, float> propSteer("Steer", "Axes", &SkateboardController::getSteer, NULL, Reflection::PropertyDescriptor::UI);
static Reflection::EventDesc<SkateboardController, void(std::string)>	event_AxisChanged(&SkateboardController::axisChangedSignal, "AxisChanged", "axis");
REFLECTION_END();


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

SkateboardController::SkateboardController()
{
	this->setName("SkateboardController");
}
    
void SkateboardController::onSteppedTouchInput()
{
    shared_ptr<SkateboardPlatform> skatePlatform = skateboardPlatform.lock();
    if(!skatePlatform)
        return;
    
    Humanoid* localHumanoid = skatePlatform->getControllingHumanoid();
    if(!localHumanoid)
        return;
    
    UserInputService* userInputService = ServiceProvider::find<UserInputService>(this);
    
    
    // todo: remove getWalkDirection call when everything uses moveDirection
    Vector3 moveDirection = localHumanoid->getWalkDirection();
    Vector2 movementVector = Vector2::zero();
    
    if (moveDirection != Vector3::zero())
    {
        movementVector = userInputService->getInputWalkDirection();
        movementVector /= userInputService->getMaxInputWalkValue();
    }
    else
    {
        moveDirection = localHumanoid->getRawMovementVector();
        movementVector = Vector2(moveDirection.x,moveDirection.z);
    }

    
    if(moveDirection != Vector3::zero())
    {
        if( movementVector.y > 0)
           setThrottle(-1);
        else if( movementVector.y < 0 )
            setThrottle(1);
        
        if( movementVector.x < -0.2 )
            setSteer(-1);
        else if( movementVector.x > 0.2 )
            setSteer(1);
    }
	else
	{
		setThrottle(0);
		setSteer(0);
	}

}

void SkateboardController::onSteppedKeyboardInput()
{
    const UserInputBase* hardwareDevice = getHardwareDevice();
	if (!hardwareDevice)
		return;
    
	NavKeys nav;
	if(DataModel* dm = DataModel::get(this))
		hardwareDevice->getNavKeys(nav,dm->getSharedSuppressNavKeys());
    
	int forwardBackward = nav.forwardBackwardArrow() + nav.forwardBackwardASDW();
	int leftRight = nav.leftRightArrow() + nav.leftRightASDW();
	setThrottle((float)forwardBackward);
	setSteer((float)-leftRight);
    
	if(isButtonBound(JUMP))
	{
		setButton(JUMP, nav.space);
	}
	else
	{
		// not bound, pass through.
		shared_ptr<SkateboardPlatform> sp = skateboardPlatform.lock();
		if(sp)
		{
			Humanoid* localHumanoid = sp->getControllingHumanoid();
			if(localHumanoid)
			{
				localHumanoid->setJump(nav.space);
			}
		}
	}
    
	bool backspaceKeyDown = nav.backspace;
	setButton(DISMOUNT, backspaceKeyDown);
	// do a dismount regardless of what script says.
	if(backspaceKeyDown)
	{
		shared_ptr<SkateboardPlatform> sp = skateboardPlatform.lock();
		if(sp)
		{
			Humanoid* localHumanoid = sp->getControllingHumanoid();
			if(localHumanoid)
			{
				localHumanoid->setJump(true);
			}
		}
	}

}

    
void SkateboardController::onStepped(const Stepped& event)
{    
    UserInputService* userInputService = ServiceProvider::find<UserInputService>(this);
    
    if(userInputService->getTouchEnabled())
        onSteppedTouchInput();
    
    if(userInputService->getKeyboardEnabled())
        onSteppedKeyboardInput();
    
}

void SkateboardController::setThrottle(float value)
{
	if(throttle != value)
	{
		throttle = value;
		axisChangedSignal("Throttle");
	}
}
void SkateboardController::setSteer(float value)
{
	if(steer != value)
	{
		steer = value;
		axisChangedSignal("Steer");
	}
}

void SkateboardController::setSkateboardPlatform(SkateboardPlatform* value)
{
	ARLASSERT(skateboardPlatform.expired());

	skateboardPlatform = shared_from<SkateboardPlatform>(value);
}


} // namespace
