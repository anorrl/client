/**
 * UserInput.cpp
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#include "stdafx.h"
#include "UserInput.h"

// Roblox Headers
#include "v8datamodel/Workspace.h"
#include "v8datamodel/GameBasicSettings.h"
#include "V8DataModel/SleepingJob.h"
#include "v8datamodel/UserInputService.h"
#include "util/NavKeys.h"
#include "rbx/TaskScheduler.h"
#include "RenderSettingsItem.h"

// Roblox Studio Headers
#include "RobloxView.h"
#include "UpdateUIManager.h"
#include "UserInputUtil.h"
#include "RobloxMainWindow.h"
#include "StudioUtilities.h"
#include "StudioDeviceEmulator.h"

LOGGROUP(UserProfile)

FASTFLAG(StudioRemoveUpdateUIThread)
DYNAMIC_FASTFLAGVARIABLE(MouseDeltaWhenNotMouseLocked, false)
DYNAMIC_FASTFLAGVARIABLE(UserInputViewportSizeFixStudio, true)

UserInput::UserInput(RobloxView *wnd, shared_ptr<ARL::DataModel> dataModel)
	:	tableSize(0)
	,	isMouseCaptured(false)
	,	leftMouseButtonDown(false)
    ,   middleMouseButtonDown(false)
	,	wrapping(false)
	,	leftMouseDown(false)
	,	rightMouseDown(false)
	,	autoMouseMove(false)
	,	wnd(wnd)
	,	isMouseInside(false)
	,	posToWrapTo(0,0)
{
	setDataModel(dataModel);
}

void UserInput::setDataModel(shared_ptr<ARL::DataModel>  dataModel)
{
	this->dataModel = dataModel;
	runService = shared_from(ARL::ServiceProvider::create<ARL::RunService>(dataModel.get()));

	sdlGameController = shared_ptr<SDLGameController>(new SDLGameController(dataModel));

	processedMouseConnection.disconnect();

	if ( ARL::UserInputService* userInputService = ARL::ServiceProvider::find<ARL::UserInputService>(dataModel.get()) )
	{
		processedMouseConnection = userInputService->processedMouseEvent.connect(boost::bind(&UserInput::mouseEventWasProcessed,this,_1,_2,_3));
	}
}

UserInput::~UserInput()
{	
	sdlGameController.reset();
	runService.reset();
}

void UserInput::mouseEventWasProcessed(bool datamodelSunkEvent, void* nativeEventObject, const shared_ptr<ARL::InputObject>& inputObject)
{
	if (inputObject)
	{
		if (inputObject->getUserInputType() == ARL::InputObject::TYPE_MOUSEBUTTON1 && inputObject->getUserInputState() == ARL::InputObject::INPUT_STATE_END)
		{
			UpdateUIManager::Instance().updateToolBars();
		}
		else if (inputObject->getUserInputType() == ARL::InputObject::TYPE_MOUSEWHEEL)
		{
			UpdateUIManager::Instance().updateAction(*UpdateUIManager::Instance().getMainWindow().zoomInAction);
			UpdateUIManager::Instance().updateAction(*UpdateUIManager::Instance().getMainWindow().zoomOutAction);
		}
	}
}

void UserInput::PostUserInputMessage(ARL::InputObject::UserInputType eventType, ARL::InputObject::UserInputState eventState, int wParam, int lParam, char extraParam, bool processed)
{
	FASTLOG4(FLog::UserInputProfile, "Pushing UI event to queue, event: %u, wParam: %u, lParam: %u, extraParam: %c", eventType, wParam, lParam, extraParam);

	if (eventType == ARL::InputObject::TYPE_MOUSEMOVEMENT)
		if (dataModel)
			dataModel->mouseStats.osMouseMove.sample();

	ProcessUserInputMessage(eventType, eventState, wParam, lParam, extraParam, processed);
}

void UserInput::ProcessUserInputMessage(
							ARL::InputObject::UserInputType eventType, 
							ARL::InputObject::UserInputState eventState,
                            int wParam,
                            int lParam,
							char extraParam,
							bool processed)
{
	bool cursorMoved = false;
	bool leftMouseUp = false;
	ARL::Vector2 wrapMouseDelta;
	G3D::Vector2 mouseDelta;
    ARL::Vector3 cursorPosition = ARL::Vector3(getCursorPositionInternal(),0);
    ARL::Vector3 deltaVec = ARL::Vector3(wrapMouseDelta.x,wrapMouseDelta.y,0);

	switch (eventType)
	{
	case ARL::InputObject::TYPE_KEYBOARD:
		{
			ARL::KeyCode keyCode = (ARL::KeyCode) wParam;
            ARL::ModCode modCode = (ARL::ModCode) lParam;

			shared_ptr<ARL::InputObject> keyInput = ARL::Creatable<ARL::Instance>::create<ARL::InputObject>(eventType, eventState, keyCode, modCode, extraParam, dataModel.get());
			sendEvent(keyInput, processed);
			
			if (wParam < NKEYSTATES)
				if (!processed)
					setKeyState(keyCode, modCode, extraParam, (eventState == ARL::InputObject::INPUT_STATE_BEGIN));
			break;
		}
	case ARL::InputObject::TYPE_MOUSEBUTTON1:
		{
            G3D::Vector2 pos((float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam));
			
            if(eventState == ARL::InputObject::INPUT_STATE_BEGIN) // button down
			{
                leftMouseButtonDown = true;
                leftMouseUp = false;
				leftMouseDown = true;
                isMouseCaptured = true;
            }
            else if(eventState == ARL::InputObject::INPUT_STATE_END) // button up
            {
                leftMouseButtonDown = false;
                leftMouseUp = true;
				leftMouseDown = false;
                autoMouseMove = true;			
                isMouseCaptured = false;
           }

			if (StudioDeviceEmulator::Instance().isEmulatingTouch() && StudioUtilities::isAvatarMode())
			{
				G3D::Vector2 pos;
				wnd->getCursorPos(&pos);
				ARL::Vector3 position = ARL::Vector3(pos,0);

				if (eventState == ARL::InputObject::INPUT_STATE_BEGIN)
				{
					touchInput = ARL::Creatable<ARL::Instance>::create<ARL::InputObject>(ARL::InputObject::TYPE_TOUCH, eventState, position, ARL::Vector3::zero(), dataModel.get());
				}
				else
				{
					touchInput->setInputState(ARL::InputObject::INPUT_STATE_END);
					touchInput->setPosition(position);
				}

				if (touchInput)
					sendEvent(touchInput);

				if (touchInput && eventState == ARL::InputObject::INPUT_STATE_END)
					touchInput.reset();
			} 
			else
			{
				sendMouseEvent(eventType, eventState, cursorPosition, deltaVec);
			}

			wrapMousePosition = pos - getWindowRect().center();

			break;
		}
	case ARL::InputObject::TYPE_MOUSEBUTTON2:
		{
			G3D::Vector2 pos((float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam));
			sendMouseEvent( eventType, eventState, cursorPosition, deltaVec);

			rightMouseDown = (eventState == ARL::InputObject::INPUT_STATE_BEGIN) ? true : false;

			wrapMousePosition = pos - getWindowRect().center();
			
			break;
		}
    case ARL::InputObject::TYPE_MOUSEBUTTON3:
		{
			G3D::Vector2 pos((float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam));
			sendMouseEvent(eventType, eventState, cursorPosition, deltaVec);

			middleMouseButtonDown = (eventState == ARL::InputObject::INPUT_STATE_BEGIN) ? true : false;
			
			wrapMousePosition = pos - getWindowRect().center();
			
			break;
		}
	case ARL::InputObject::TYPE_MOUSEMOVEMENT:
	{
		ARL::UserInputService* userInputService = ARL::ServiceProvider::find<ARL::UserInputService>(dataModel.get());

		G3D::Vector2 windowCursorPos = getWindowsCursorPositionInternal() - getWindowRect().center();

		if ( windowCursorPos == wrapMousePosition &&
				(userInputService->getMouseWrapMode() == ARL::UserInputService::WRAP_CENTER || userInputService->getMouseWrapMode() == ARL::UserInputService::WRAP_NONEANDCENTER))
					return;

		G3D::Vector2 pos;
		wnd->getCursorPos(&pos);
		ARL::Vector3 position = ARL::Vector3(pos,0);

		if (StudioDeviceEmulator::Instance().isEmulatingTouch() && StudioUtilities::isAvatarMode() && touchInput)
		{
			touchInput->setInputState(ARL::InputObject::INPUT_STATE_CHANGE);
			touchInput->setPosition(position);

			if (leftMouseDown)
				sendEvent(touchInput);
		}
		
		//if application doesn't have focus then just send mouse move events, do not do any mouse wrapping
		if (!wnd->hasApplicationFocus())
		{
			sendMouseEvent( ARL::InputObject::TYPE_MOUSEMOVEMENT, ARL::InputObject::INPUT_STATE_CHANGE, position, deltaVec);
			return;
		}

		mouseDelta = G3D::Vector2((float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam));

		autoMouseMove = false;
		cursorMoved = true;

		if(userInputService->getMouseWrapMode() != ARL::UserInputService::WRAP_HYBRID)
		   doWrapMouse(mouseDelta, wrapMouseDelta);
		
		break;	
	}
	case ARL::InputObject::TYPE_MOUSEWHEEL:
		{
			int zDelta = (int) wParam; // wheel rotation
			G3D::Vector2 pos((float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam)); 
            ARL::Vector3 wheelPos = ARL::Vector3(pos,zDelta);
            
			sendMouseEvent(ARL::InputObject::TYPE_MOUSEWHEEL, ARL::InputObject::INPUT_STATE_CHANGE, wheelPos, deltaVec);
			
			break;
		}
	case ARL::InputObject::TYPE_FOCUS:
		{
			shared_ptr<ARL::InputObject> event = ARL::Creatable<ARL::Instance>::create<ARL::InputObject>(ARL::InputObject::TYPE_FOCUS, eventState, ARL::Vector3(getCursorPositionInternal(),0), ARL::Vector3::zero(), dataModel.get());

			sendEvent(event);
			break;
		}
	default:
		{
			break;
		}
	}
	
	/////////// DONE PROCESSING ////////////////
	postProcessUserInput(cursorMoved, leftMouseUp, wrapMouseDelta, mouseDelta);
}

void UserInput::postProcessUserInput(bool cursorMoved, bool leftMouseUp, ARL::Vector2 wrapMouseDelta, ARL::Vector2 mouseDelta)
{
    ARL::Vector3 deltaVec;

	if (DFFlag::MouseDeltaWhenNotMouseLocked)
	{
		deltaVec = ARL::Vector3(mouseDelta.x, mouseDelta.y,0);
	}
	else
	{
		deltaVec = ARL::Vector3(wrapMouseDelta.x,wrapMouseDelta.y,0);
	}

	ARL::UserInputService* userInputService = ARL::ServiceProvider::find<ARL::UserInputService>(dataModel.get());

	// kind of a hack to allows us to pan when mouse isn't moving
	if(userInputService->getMouseWrapMode() == ARL::UserInputService::WRAP_HYBRID && !dataModel->getMouseOverGui())
	{
		doWrapHybrid(cursorMoved,
					 leftMouseUp,
					 wrapMouseDelta,
					 wrapMousePosition,
					 posToWrapTo);
	}
	else if(ARL::GameBasicSettings::singleton().inMousepanMode())
	{
		if(leftMouseButtonDown && !rightMouseDown && !middleMouseButtonDown && ARL::GameBasicSettings::singleton().getCanMousePan())
			userInputService->setMouseWrapMode(ARL::UserInputService::WRAP_CENTER);
		else if(!rightMouseDown && !middleMouseButtonDown)
			userInputService->setMouseWrapMode(ARL::UserInputService::WRAP_AUTO);
	}
	
	if(userInputService->getMouseWrapMode() == ARL::UserInputService::WRAP_HYBRID)
		doWrapMouse(mouseDelta, wrapMouseDelta);
	
	ARL::Vector3 cursorPos = ARL::Vector3(getCursorPositionInternal(),0);

	if (wrapMouseDelta != ARL::Vector2::zero())
	{
		if (userInputService->getMouseWrapMode() != ARL::UserInputService::WRAP_NONEANDCENTER &&
			userInputService->getMouseWrapMode() != ARL::UserInputService::WRAP_NONE)
		{
			if (dataModel->getWorkspace())
			{
				if (userInputService)
				{
					userInputService->wrapCamera(wrapMouseDelta);
				}		
			}
		}
		
		sendMouseEvent(ARL::InputObject::TYPE_MOUSEDELTA,ARL::InputObject::INPUT_STATE_CHANGE, cursorPos, deltaVec);
		sendMouseEvent(ARL::InputObject::TYPE_MOUSEMOVEMENT,ARL::InputObject::INPUT_STATE_CHANGE, cursorPos, deltaVec);
	}
	else if (cursorMoved)
	{
		sendMouseEvent(ARL::InputObject::TYPE_MOUSEMOVEMENT,ARL::InputObject::INPUT_STATE_CHANGE, cursorPos, deltaVec);
	}
}

void UserInput::sendMouseEvent(ARL::InputObject::UserInputType mouseEventType,
					ARL::InputObject::UserInputState mouseEventState,
					ARL::Vector3& position,
					ARL::Vector3& delta)
{
	FASTLOG1(FLog::UserInputProfile, "Processing mouse event from the queue, event: %u", mouseEventType);

	shared_ptr<ARL::InputObject> mouseEventObject;
	if( inputObjectMap.find(mouseEventType) == inputObjectMap.end() )
	{
		mouseEventObject = ARL::Creatable<ARL::Instance>::create<ARL::InputObject>(mouseEventType, mouseEventState, position, delta, dataModel.get());
		inputObjectMap[mouseEventType] = mouseEventObject;
	}
	else
	{
		mouseEventObject = inputObjectMap[mouseEventType];

		// make sure we aren't sending redundant move events
		// only send if we have new position and a delta from last position
		if (mouseEventType == ARL::InputObject::TYPE_MOUSEMOVEMENT)
		{
			ARL::Vector3 currentMouseMovePos = mouseEventObject->getRawPosition();
			if (currentMouseMovePos == position &&
				delta == ARL::Vector3::zero())
			{
				return;
			}
		}

		mouseEventObject->setInputState(mouseEventState);
		mouseEventObject->setPosition(position);
		mouseEventObject->setDelta(delta);
		inputObjectMap[mouseEventType] = mouseEventObject;
	}

	
	if( ARL::UserInputService* userInputService = ARL::ServiceProvider::find<ARL::UserInputService>(dataModel.get()) )
		userInputService->fireInputEvent(mouseEventObject, NULL);
		
}

void UserInput::sendEvent(shared_ptr<ARL::InputObject> event, bool processed)
{
	if( ARL::UserInputService* userInputService = ARL::ServiceProvider::find<ARL::UserInputService>(dataModel.get()) )
		userInputService->fireInputEvent(event, NULL, processed);
}

bool UserInput::isFullScreenMode() const
{
	return false;
}

G3D::Rect2D UserInput::getWindowRect() const
{
	float width = wnd->getWidth();
	float height = wnd->getHeight();
	if (DFFlag::UserInputViewportSizeFixStudio) {
		ARL::Camera* cam = this->dataModel.get()->getWorkspace()->getCamera();
		width = cam->getViewportWidth();
		height = cam->getViewportHeight();
	}
	G3D::Rect2D answer(G3D::Vector2(width, height));
	return answer;
}

G3D::Vector2int16 UserInput::getWindowSize() const
{
	G3D::Rect2D windowRect = getWindowRect();
	return G3D::Vector2int16((G3D::int16) windowRect.width(), (G3D::int16) windowRect.height());
}

ARL::TextureProxyBaseRef UserInput::getGameCursor(ARL::Adorn* adorn)
{
	if (isMouseInside)
	{
		if (ARL::UserInputService* inputService = ARL::ServiceProvider::find<ARL::UserInputService>(dataModel.get()))
		{
			ARL::TextureId mouseTexture = inputService->getCurrentMouseIcon();

			if (!mouseTexture.isNull())
			{
				bool waitingCurrentCursor;
				return adorn->createTextureProxy(mouseTexture, waitingCurrentCursor, false, "Mouse Cursor");
			}
		}
		return UserInputBase::getGameCursor(adorn);
	}
	
	return ARL::TextureProxyBaseRef();
}

void UserInput::doWrapHybrid(bool, bool leftMouseUp, G3D::Vector2& wrapMouseDelta, G3D::Vector2& wrapMousePosition, G3D::Vector2& posToWrapTo)
{

}

bool UserInput::movementKeysDown()
{
	return (	keyDownInternal(ARL::SDLK_w)	|| keyDownInternal(ARL::SDLK_s) || keyDownInternal(ARL::SDLK_a) || keyDownInternal(ARL::SDLK_d)
			||	keyDownInternal(ARL::SDLK_UP)|| keyDownInternal(ARL::SDLK_DOWN) || keyDownInternal(ARL::SDLK_LEFT) || keyDownInternal(ARL::SDLK_RIGHT) );
}

bool UserInput::areEditModeMovementKeysDown()
{
    return ( movementKeysDown() || keyDownInternal(ARL::SDLK_q) || keyDownInternal(ARL::SDLK_e) );
}

G3D::Vector2 UserInput::getGameCursorPositionInternal()
{
	return getWindowRect().center() + wrapMousePosition;
}

G3D::Vector2 UserInput::getGameCursorPositionExpandedInternal()
{
	return getWindowRect().center() + ARL::Math::expandVector2(wrapMousePosition, 10);
}


// will be called repeatedly. ignore repeated calls.
void UserInput::onMouseInside()
{
	G3D::Vector2 pos;
	wnd->getCursorPos(&pos);	
	//wrapMousePosition = ARL::Math::expandVector2(pos - getWindowRect().center(), -10);	// i.e. - pull towards the origin - remove chatter
	wrapMousePosition = pos - getWindowRect().center();

	if(isMouseInside)
		return; // we know. ignore.

	if (ARL::UserInputService* inputService = ARL::ServiceProvider::find<ARL::UserInputService>(dataModel.get()))
	{
		if (inputService->getMouseWrapMode() == ARL::UserInputService::WRAP_NONEANDCENTER)
		{
			centerCursor();
		}
	}

	isMouseInside = true;

};

void UserInput::onMouseLeave() 
{ 
	if (!isMouseInside)
		return;

	if (rightMouseDown)
		PostUserInputMessage(ARL::InputObject::TYPE_MOUSEBUTTON2, ARL::InputObject::INPUT_STATE_END, 0, 0);	

	isMouseInside = false;
};

G3D::Vector2 UserInput::getWindowsCursorPositionInternal()
{
	G3D::Vector2 pt;
	wnd->getCursorPos(&pt);
	return pt;	
}

G3D::Vector2 UserInput::getCursorPosition()
{
	return getCursorPositionInternal();
}

G3D::Vector2 UserInput::getCursorPositionInternal()
{
	return getGameCursorPositionInternal();
}

bool UserInput::keyDownInternal(ARL::KeyCode code) const
{
	if( ARL::UserInputService* userInputService = ARL::ServiceProvider::find<ARL::UserInputService>(dataModel.get()) )
		return userInputService->isKeyDown(code);

	return false;
}

bool UserInput::keyDown(ARL::KeyCode code) const
{
	return keyDownInternal(code);
}

void UserInput::setKeyState(ARL::KeyCode code, ARL::ModCode modCode, char modifiedKey, bool isDown)
{
	if(ARL::UserInputService* userInputService = ARL::ServiceProvider::find<ARL::UserInputService>(dataModel.get()))
		userInputService->setKeyState( code, modCode, modifiedKey, isDown );
}

void UserInput::resetKeyState()
{
	if(ARL::UserInputService* userInputService = ARL::ServiceProvider::find<ARL::UserInputService>(dataModel.get()))
		userInputService->resetKeyState();
}

bool preventWrapMouse()
{
	// unlikely to get here
	return true;
}

void UserInput::doWrapMouse(const G3D::Vector2& delta, G3D::Vector2& wrapMouseDelta)
{
	ARL::Vector2 savedPos = getGameCursorPositionInternal();
	ARL::UserInputService* userInputService = ARL::ServiceProvider::find<ARL::UserInputService>(dataModel.get());
	
	switch (userInputService->getMouseWrapMode())
	{ 
	case ARL::UserInputService::WRAP_NONE:
	case ARL::UserInputService::WRAP_NONEANDCENTER: 
	case ARL::UserInputService::WRAP_CENTER: // intentional fall-thru
		{
            UserInputUtil::wrapMouseCenter(delta,
				wrapMouseDelta,
				this->wrapMousePosition);
			break;
		}
	case ARL::UserInputService::WRAP_HYBRID:
		{
			UserInputUtil::wrapMousePos(delta,
										wrapMouseDelta,
										wrapMousePosition,
										getWindowSize(),
										posToWrapTo,
										autoMouseMove);
			break;
		}
	case ARL::UserInputService::WRAP_AUTO:
		{
			if (movementKeysDown() || isMouseCaptured) {		// 1. If movement keys are down - keep the mouse in the window
				UserInputUtil::wrapMouseBorderLock(delta,			// 2. If the left mouse button is down (we are dragging) - keep in the window
					wrapMouseDelta, 
					this->wrapMousePosition, 
					getWindowSize());
			}
			else if (preventWrapMouse()) {							// 3. If using toolbox, prevent wrapping
				UserInputUtil::wrapMouseNone(delta, 
					wrapMouseDelta, 
					this->wrapMousePosition);
			}
			else if (isFullScreenMode()) {							// 4. OK - we're in PlayMode (i.e. character)
				UserInputUtil::wrapFullScreen(delta,
					wrapMouseDelta, 
					this->wrapMousePosition, 
					getWindowSize());
			}
			else {
				// We no longer want mouse wrap and camera auto-pan at the horizontal window extents.
				UserInputUtil::wrapMouseNone(delta, 
					wrapMouseDelta, 
					this->wrapMousePosition);
			}
			break;
		}
	}
	
	if (userInputService->getMouseWrapMode() != ARL::UserInputService::WRAP_HYBRID)
	{
		ARL::Vector2 pos = getGameCursorPositionInternal();
		wnd->setCursorPos(&pos, leftMouseButtonDown, pos == savedPos);
	}	
}
