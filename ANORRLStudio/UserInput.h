/**
 * UserInput.h
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#pragma once

// Roblox Headers
#include "util/UserInputBase.h"
#include "util/RunStateOwner.h"
#include "util/Rect.h"
#include "V8DataModel/InputObject.h"
#include "SDLGameController.h"

class RobloxView;

namespace ARL {
	class DataModel;
}

class RobloxCriticalSection
{
private:
public:
	int callers;
	std::string recentCaller;
	std::string olderCaller;
	RobloxCriticalSection() 
		: callers(0)
	{}
};

class RobloxCritSecLoc
{
private:
	RobloxCriticalSection& robloxCriticalSection;

public:
	RobloxCritSecLoc( RobloxCriticalSection& cs, const std::string& location) 
		: robloxCriticalSection(cs)
	{
/*
	#ifdef __ARL_NOT_RELEASE			// should compile away the string manipulations
		ARLASSERT(robloxCriticalSection.callers == 0);					// Show THESE ASSERTS TO David/Erik - they imply multiple entry points to User Input Object - trying to track down
		robloxCriticalSection.callers++;
		robloxCriticalSection.olderCaller = robloxCriticalSection.recentCaller;
		robloxCriticalSection.recentCaller = location;
	#endif
*/
	}
	
	~RobloxCritSecLoc()
	{
/*
	#ifdef __ARL_NOT_RELEASE			// should compile away the string manipulations
		ARLASSERT(robloxCriticalSection.callers == 1);
		robloxCriticalSection.callers--;
		robloxCriticalSection.recentCaller = "";
		robloxCriticalSection.olderCaller = "";
	#endif
*/
	}
};


class UserInput 
	: public ARL::UserInputBase
{
private:
	int debugI;

	mutable RobloxCriticalSection diSection;

	rbx::signals::scoped_connection processedMouseConnection;

	size_t tableSize;

	// Mouse Stuff
	bool isMouseCaptured;			// poor man's tracker of button state
	bool leftMouseButtonDown;
	bool middleMouseButtonDown;
    bool rightMouseButtonDown;
	G3D::Vector2 wrapMousePosition;		// in normalized coordinates (center is 0,0. radius is getWrapRadius)
	bool wrapping;
	bool leftMouseDown;
	bool rightMouseDown;
	bool autoMouseMove;

	// Keyboard Stuff
	int externallyForcedKeyDown;		// + this from exteranal source (like a gui button)

	class RobloxView * wnd;
	shared_ptr<ARL::RunService> runService;

	// InputObject stuff
	boost::unordered_map<ARL::InputObject::UserInputType,shared_ptr<ARL::InputObject> > inputObjectMap;

	shared_ptr<SDLGameController> sdlGameController;


	////////////////////////////////
	//
	// Events


	void sendEvent(shared_ptr<ARL::InputObject> event, bool processed = false);
	void sendMouseEvent(ARL::InputObject::UserInputType mouseEventType,
		ARL::InputObject::UserInputState mouseEventState,
		ARL::Vector3& position, ARL::Vector3& delta);

	////////////////////////////////////
	// 
	// Keyboard Mouse
	bool isMouseInside;
	
	G3D::Vector2 posToWrapTo;

	// Studio mobile emulation
	shared_ptr<ARL::InputObject> touchInput;

	// window stuff
	ARL::Vector2int16 getWindowSize() const;
	G3D::Rect2D getWindowRect() const;			// { return windowRect; }
	bool isFullScreenMode() const;
	bool movementKeysDown();
	bool keyDownInternal(ARL::KeyCode code) const;

	void doWrapMouse(const G3D::Vector2& delta, G3D::Vector2& wrapMouseDelta);
	
	G3D::Vector2 getGameCursorPositionInternal();
	G3D::Vector2 getGameCursorPositionExpandedInternal();	// prevent hysteresis
	G3D::Vector2 getWindowsCursorPositionInternal();
	ARL::Vector2 getCursorPositionInternal();
		
	void doDiagnostics();

	void postProcessUserInput(bool cursorMoved, bool leftMouseUp, ARL::Vector2 wrapMouseDelta, ARL::Vector2 mouseDelta);
	void mouseEventWasProcessed(bool datamodelSunkEvent, void* nativeEventObject, const shared_ptr<ARL::InputObject>& inputObject);

	void renderStepped();

public:
	shared_ptr<ARL::DataModel> dataModel;

//	const static int WM_CALL_SETFOCUS = WM_USER + 187;
//	HCURSOR hInvisibleCursor;

	////////////////////////////////
	//
	// UserInputBase

	/*implement*/ ARL::Vector2 getCursorPosition();
	
	// todo: no longer used, remove this
	void doWrapHybrid(bool cursorMoved, bool leftMouseUp, G3D::Vector2& wrapMouseDelta, G3D::Vector2& wrapMousePosition, G3D::Vector2& posToWrapTo);

	/*implement*/ bool keyDown(ARL::KeyCode code) const;
    /*implement*/ void setKeyState(ARL::KeyCode code, ARL::ModCode modCode, char modifiedKey, bool isDown);
    bool isMiddleMouseDown() const { return middleMouseButtonDown; }
    bool isLeftMouseDown() const { return leftMouseButtonDown; }

    bool areEditModeMovementKeysDown();

	/*implement*/ void centerCursor() {wrapMousePosition = ARL::Vector2::zero();}

	/*override*/ ARL::TextureProxyBaseRef getGameCursor(ARL::Adorn* adorn);

	void PostUserInputMessage(ARL::InputObject::UserInputType eventType, ARL::InputObject::UserInputState eventState, int wParam, int lParam, char extraParam = 0, bool processed = false);
	// Call this only within a DataModel lock:
	void ProcessUserInputMessage(ARL::InputObject::UserInputType eventType, ARL::InputObject::UserInputState eventState, int wParam, int lParam, char extraParam = 0, bool processed = false);

	UserInput(RobloxView *wnd, shared_ptr<ARL::DataModel> dataModel);
	~UserInput();

	void setDataModel(shared_ptr<ARL::DataModel> dataModel);

	void onMouseInside();
	void onMouseLeave();

	void resetKeyState();
};
