/**
 * UserInputUtil.cpp
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#include "stdafx.h"
#include "UserInputUtil.h"
#include "Util/Rect.h"
#include "util/standardout.h"
#include "V8DataModel/GameBasicSettings.h"

#include <dinput.h>

using ARL::Vector2;
using ARL::Rect;


const float UserInputUtil::HybridSensitivity = 15.0f;
const float UserInputUtil::MouseTug = 20.0f;

bool UserInputUtil::isCtrlDown(ARL::ModCode modCode)
{
	return (modCode == ARL::KMOD_LCTRL || modCode == ARL::KMOD_RCTRL);
}


// horizontal - will keep doing deltas
// vertical - will peg inset 2 pixels
//
void UserInputUtil::wrapFullScreen(const Vector2& delta,
										Vector2& wrapMouseDelta, 
										Vector2& wrapMousePosition,
										const Vector2& windowSize)
{
	float wrapPositionY = wrapMousePosition.y + delta.y;
	wrapMouseBorderLock(delta, wrapMouseDelta, wrapMousePosition, windowSize);
	wrapMouseDelta.y = 0.0;
	wrapMousePosition.y = wrapPositionY;
	float halfHeight = (windowSize.y * 0.5);

	float wrapPositionX = wrapMousePosition.x;
	float halfWidth = (windowSize.x * 0.5);
	wrapMousePosition.x = G3D::clamp(wrapPositionX,-halfWidth,halfWidth);

	// This is changed so we can pop out into the chat area below
	wrapMousePosition.y = G3D::clamp(wrapMousePosition.y, -halfHeight, halfHeight);
	//wrapMousePosition.y = std::max(-halfHeight, wrapMousePosition.y);

	// Setting this to zero prevents the camera from panning automatically near the extents of the screen.
	wrapMouseDelta = Vector2::zero();
}

void UserInputUtil::wrapMouseHorizontalTransition(const Vector2& delta,
										Vector2& wrapMouseDelta, 
										Vector2& wrapMousePosition,
										const Vector2& windowSize)
{
	float wrapPositionY = wrapMousePosition.y + delta.y;
	wrapMouseBorderTransition(delta, wrapMouseDelta, wrapMousePosition, windowSize);
	wrapMouseDelta.y = 0.0;
	wrapMousePosition.y = wrapPositionY;
}


void UserInputUtil::wrapMouseBorder(const Vector2& delta,
									Vector2& wrapMouseDelta, 
									Vector2& wrapMousePosition,
									const Vector2& windowSize,
									const int borderWidth,
									const float creepFactor)

{
	Vector2 halfSize = windowSize * 0.5;
	Rect inner = Rect(-halfSize, halfSize).inset(borderWidth);	// in Wrap Coordinates
	Vector2 oldPosition = wrapMousePosition;
	inner.unionWith(oldPosition);					// now union of the border and old position - ratchet

	Vector2 newPositionUnclamped = oldPosition + delta;
	Vector2 newPositionClamped = inner.clamp(newPositionUnclamped);
	
	Vector2 positiveDistanceInBorder = newPositionUnclamped - newPositionClamped;


	if(!ARL::GameBasicSettings::singleton().inMousepanMode())
		wrapMousePosition = newPositionClamped + (positiveDistanceInBorder * creepFactor);
	wrapMouseDelta += positiveDistanceInBorder;
}

void UserInputUtil::wrapMouseBorderLock(const Vector2& delta,
									Vector2& wrapMouseDelta, 
									Vector2& wrapMousePosition,
									const Vector2& windowSize)
{
	wrapMouseBorder(
		delta,
		wrapMouseDelta,
		wrapMousePosition,
		windowSize, 
		6,
		0.0f);
}

void UserInputUtil::wrapMouseBorderTransition(const Vector2& delta,
									Vector2& wrapMouseDelta, 
									Vector2& wrapMousePosition,
									const Vector2& windowSize)
{
	wrapMouseBorder(
		delta,
		wrapMouseDelta,
		wrapMousePosition,
		windowSize, 
		20,
		0.05f);
}

void UserInputUtil::wrapMouseNone(const Vector2& delta, 
									Vector2& wrapMouseDelta, 
									Vector2& wrapMousePosition)
{
	wrapMouseDelta = Vector2::zero();
	wrapMousePosition += delta;
}

void UserInputUtil::wrapMouseCenter(const Vector2& delta, 
									Vector2& wrapMouseDelta, 
									Vector2& wrapMousePosition)
{
	wrapMouseDelta += delta;
	// don't move the cursor....
	// wrapMousePosition = G3D::Vector2::zero();
	
}

void UserInputUtil::wrapMousePos(const Vector2& delta, 
									Vector2& wrapMouseDelta, 
									Vector2& wrapMousePosition,
									const Vector2& windowSize,
									Vector2& posToWrapTo,
									bool autoMoveMouse)
{
	if(posToWrapTo.length() > 2)
	{
		Vector2 windowDelta = posToWrapTo/windowSize;
		wrapMouseDelta = windowDelta * HybridSensitivity;
		posToWrapTo -= (wrapMouseDelta * HybridSensitivity * 0.266f); // 0.266 is a tuning constant

		float xDiff = std::abs(wrapMousePosition.x/wrapMousePosition.length()) * 0.3f;
		float yDiff = std::abs(wrapMousePosition.y/wrapMousePosition.length()) * 0.4f;

		if(autoMoveMouse)
		{
			if(wrapMousePosition.x < 0)
			{
				wrapMousePosition.x += xDiff * wrapMouseDelta.length() * MouseTug;
				if(wrapMousePosition.x > 0)
					wrapMousePosition.x = 0;
			}
			else if (wrapMousePosition.x > 0)
			{
				wrapMousePosition.x -= xDiff * wrapMouseDelta.length()* MouseTug;
				if(wrapMousePosition.x < 0)
					wrapMousePosition.x = 0;
			}
			

			if(wrapMousePosition.y < 0)
			{
				wrapMousePosition.y += yDiff * wrapMouseDelta.length() * MouseTug;
				if(wrapMousePosition.y > 0)
					wrapMousePosition.y = 0;
			}
			else if (wrapMousePosition.y > 0)
			{
				wrapMousePosition.y -= yDiff * wrapMouseDelta.length() * MouseTug;
				if(wrapMousePosition.y < 0)
					wrapMousePosition.y = 0;
			}
		}

	}
	
	wrapMousePosition += delta;		
}


void UserInputUtil::wrapMouseHorizontalCenter(const Vector2& delta, 
											Vector2& wrapMouseDelta, 
											Vector2& wrapMousePosition)
{
	wrapMouseDelta.x += delta.x;
//	wrapMousePosition = G3D::Vector2::zero();
}



G3D::Vector2 UserInputUtil::didodToVector2(const DIDEVICEOBJECTDATA& didod)
{
	G3D::Vector2 answer(0,0);
	float data = (float) ((int)didod.dwData);

	if (didod.dwOfs==DIMOFS_X) {
		answer.x = data;
	}
	else {
		answer.y = data;
	}
	return answer;
}


// Maps DIK_* to ARL::SDLK_* 
ARL::KeyCode UserInputUtil::directInputToKeyCode(DWORD diKey)
{
	ARLASSERT(diKey>=0);
	ARLASSERT(diKey<256);

	static ARL::KeyCode keymap[256];
	static bool initialized = false;
	if (!initialized)
	{
		for ( int i=0; i<256; ++i )
			keymap[i] = ARL::SDLK_UNKNOWN;

		keymap[DIK_ESCAPE] = ARL::SDLK_ESCAPE;
		keymap[DIK_1] = ARL::SDLK_1;
		keymap[DIK_2] = ARL::SDLK_2;
		keymap[DIK_3] = ARL::SDLK_3;
		keymap[DIK_4] = ARL::SDLK_4;
		keymap[DIK_5] = ARL::SDLK_5;
		keymap[DIK_6] = ARL::SDLK_6;
		keymap[DIK_7] = ARL::SDLK_7;
		keymap[DIK_8] = ARL::SDLK_8;
		keymap[DIK_9] = ARL::SDLK_9;
		keymap[DIK_0] = ARL::SDLK_0;
		keymap[DIK_MINUS] = ARL::SDLK_MINUS;
		keymap[DIK_EQUALS] = ARL::SDLK_EQUALS;
		keymap[DIK_BACK] = ARL::SDLK_BACKSPACE;
		keymap[DIK_TAB] = ARL::SDLK_TAB;
		keymap[DIK_Q] = ARL::SDLK_q;
		keymap[DIK_W] = ARL::SDLK_w;
		keymap[DIK_E] = ARL::SDLK_e;
		keymap[DIK_R] = ARL::SDLK_r;
		keymap[DIK_T] = ARL::SDLK_t;
		keymap[DIK_Y] = ARL::SDLK_y;
		keymap[DIK_U] = ARL::SDLK_u;
		keymap[DIK_I] = ARL::SDLK_i;
		keymap[DIK_O] = ARL::SDLK_o;
		keymap[DIK_P] = ARL::SDLK_p;

		keymap[DIK_LBRACKET] = ARL::SDLK_LEFTBRACKET;
		keymap[DIK_AT] = ARL::SDLK_AT;
		keymap[DIK_RBRACKET] = ARL::SDLK_RIGHTBRACKET;
		keymap[DIK_PREVTRACK] = ARL::SDLK_EQUALS;
		keymap[DIK_COLON] = ARL::SDLK_COLON;
		keymap[DIK_KANJI] = ARL::SDLK_BACKQUOTE; // weird key mapping....

		keymap[DIK_RETURN] = ARL::SDLK_RETURN;
		keymap[DIK_LCONTROL] = ARL::SDLK_LCTRL;
		keymap[DIK_A] = ARL::SDLK_a;
		keymap[DIK_S] = ARL::SDLK_s;
		keymap[DIK_D] = ARL::SDLK_d;
		keymap[DIK_F] = ARL::SDLK_f;
		keymap[DIK_G] = ARL::SDLK_g;
		keymap[DIK_H] = ARL::SDLK_h;
		keymap[DIK_J] = ARL::SDLK_j;
		keymap[DIK_K] = ARL::SDLK_k;
		keymap[DIK_L] = ARL::SDLK_l;
		keymap[DIK_SEMICOLON] = ARL::SDLK_SEMICOLON;
		keymap[DIK_APOSTROPHE] = ARL::SDLK_QUOTE;
		keymap[DIK_GRAVE] = ARL::SDLK_BACKQUOTE;
		keymap[DIK_LSHIFT] = ARL::SDLK_LSHIFT;
		keymap[DIK_BACKSLASH] = ARL::SDLK_BACKSLASH;
		keymap[DIK_OEM_102] = ARL::SDLK_BACKSLASH;
		keymap[DIK_Z] = ARL::SDLK_z;
		keymap[DIK_X] = ARL::SDLK_x;
		keymap[DIK_C] = ARL::SDLK_c;
		keymap[DIK_V] = ARL::SDLK_v;
		keymap[DIK_B] = ARL::SDLK_b;
		keymap[DIK_N] = ARL::SDLK_n;
		keymap[DIK_M] = ARL::SDLK_m;
		keymap[DIK_COMMA] = ARL::SDLK_COMMA;
		keymap[DIK_PERIOD] = ARL::SDLK_PERIOD;
		keymap[DIK_SLASH] = ARL::SDLK_SLASH;
		keymap[DIK_RSHIFT] = ARL::SDLK_RSHIFT;
		keymap[DIK_MULTIPLY] = ARL::SDLK_KP_MULTIPLY;
		keymap[DIK_LMENU] = ARL::SDLK_LALT;
		keymap[DIK_SPACE] = ARL::SDLK_SPACE;
		keymap[DIK_CAPITAL] = ARL::SDLK_CAPSLOCK;
		keymap[DIK_F1] = ARL::SDLK_F1;
		keymap[DIK_F2] = ARL::SDLK_F2;
		keymap[DIK_F3] = ARL::SDLK_F3;
		keymap[DIK_F4] = ARL::SDLK_F4;
		keymap[DIK_F5] = ARL::SDLK_F5;
		keymap[DIK_F6] = ARL::SDLK_F6;
		keymap[DIK_F7] = ARL::SDLK_F7;
		keymap[DIK_F8] = ARL::SDLK_F8;
		keymap[DIK_F9] = ARL::SDLK_F9;
		keymap[DIK_F10] = ARL::SDLK_F10;
		keymap[DIK_NUMLOCK] = ARL::SDLK_NUMLOCK;
		keymap[DIK_SCROLL] = ARL::SDLK_SCROLLOCK;
		keymap[DIK_NUMPAD7] = ARL::SDLK_KP7;
		keymap[DIK_NUMPAD8] = ARL::SDLK_KP8;
		keymap[DIK_NUMPAD9] = ARL::SDLK_KP9;
		keymap[DIK_SUBTRACT] = ARL::SDLK_KP_MINUS;
		keymap[DIK_NUMPAD4] = ARL::SDLK_KP4;
		keymap[DIK_NUMPAD5] = ARL::SDLK_KP5;
		keymap[DIK_NUMPAD6] = ARL::SDLK_KP6;
		keymap[DIK_ADD] = ARL::SDLK_KP_PLUS;
		keymap[DIK_NUMPAD1] = ARL::SDLK_KP1;
		keymap[DIK_NUMPAD2] = ARL::SDLK_KP2;
		keymap[DIK_NUMPAD3] = ARL::SDLK_KP3;
		keymap[DIK_NUMPAD0] = ARL::SDLK_KP0;
		keymap[DIK_DECIMAL] = ARL::SDLK_KP_PERIOD;
		keymap[DIK_F11] = ARL::SDLK_F11;
		keymap[DIK_F12] = ARL::SDLK_F12;
		keymap[DIK_F13] = ARL::SDLK_F13;
		keymap[DIK_F14] = ARL::SDLK_F14;
		keymap[DIK_F15] = ARL::SDLK_F15;
		keymap[DIK_NUMPADEQUALS] = ARL::SDLK_KP_EQUALS;
		keymap[DIK_NUMPADENTER] = ARL::SDLK_KP_ENTER;
		keymap[DIK_RCONTROL] = ARL::SDLK_RCTRL;
		keymap[DIK_DIVIDE] = ARL::SDLK_KP_DIVIDE;
		keymap[DIK_SYSRQ] = ARL::SDLK_SYSREQ;
		keymap[DIK_RMENU] = ARL::SDLK_RALT;
		keymap[DIK_PAUSE] = ARL::SDLK_PAUSE;
		keymap[DIK_HOME] = ARL::SDLK_HOME;
		keymap[DIK_UP] = ARL::SDLK_UP;
		keymap[DIK_PRIOR] = ARL::SDLK_PAGEUP;
		keymap[DIK_LEFT] = ARL::SDLK_LEFT;
		keymap[DIK_RIGHT] = ARL::SDLK_RIGHT;
		keymap[DIK_END] = ARL::SDLK_END;
		keymap[DIK_DOWN] = ARL::SDLK_DOWN;
		keymap[DIK_NEXT] = ARL::SDLK_PAGEDOWN;
		keymap[DIK_INSERT] = ARL::SDLK_INSERT;
		keymap[DIK_DELETE] = ARL::SDLK_DELETE;
		keymap[DIK_LWIN] = ARL::SDLK_LMETA;
		keymap[DIK_RWIN] = ARL::SDLK_RMETA;
		keymap[DIK_APPS] = ARL::SDLK_MENU;
		initialized = true;
	}

	return keymap[diKey];
}

// Maps ARL::ARL::SDLK_* to DIK_* 
DWORD UserInputUtil::keyCodeToDirectInput(ARL::KeyCode keyCode)
{
	static DWORD keymap[ARL::SDLK_LAST];
	static bool initialized = false;
	if (!initialized)
	{
		for ( int i=0; i<ARL::SDLK_LAST; ++i )
			keymap[i] = 0;

		keymap[ARL::SDLK_ESCAPE] = DIK_ESCAPE;
		keymap[ARL::SDLK_1] = DIK_1;
		keymap[ARL::SDLK_2] = DIK_2;
		keymap[ARL::SDLK_3] = DIK_3;
		keymap[ARL::SDLK_4] = DIK_4;
		keymap[ARL::SDLK_5] = DIK_5;
		keymap[ARL::SDLK_6] = DIK_6;
		keymap[ARL::SDLK_7] = DIK_7;
		keymap[ARL::SDLK_8] = DIK_8;
		keymap[ARL::SDLK_9] = DIK_9;
		keymap[ARL::SDLK_0] = DIK_0;
		keymap[ARL::SDLK_MINUS] = DIK_MINUS;
		keymap[ARL::SDLK_EQUALS] = DIK_EQUALS;
		keymap[ARL::SDLK_BACKSPACE] = DIK_BACK;
		keymap[ARL::SDLK_TAB] = DIK_TAB;
		keymap[ARL::SDLK_q] = DIK_Q;
		keymap[ARL::SDLK_w] = DIK_W;
		keymap[ARL::SDLK_e] = DIK_E;
		keymap[ARL::SDLK_r] = DIK_R;
		keymap[ARL::SDLK_t] = DIK_T;
		keymap[ARL::SDLK_y] = DIK_Y;
		keymap[ARL::SDLK_u] = DIK_U;
		keymap[ARL::SDLK_i] = DIK_I;
		keymap[ARL::SDLK_o] = DIK_O;
		keymap[ARL::SDLK_p] = DIK_P;
		keymap[ARL::SDLK_LEFTBRACKET] = DIK_LBRACKET;
		keymap[ARL::SDLK_RIGHTBRACKET] = DIK_RBRACKET;
		keymap[ARL::SDLK_RETURN] = DIK_RETURN;
		keymap[ARL::SDLK_LCTRL] = DIK_LCONTROL;
		keymap[ARL::SDLK_a] = DIK_A;
		keymap[ARL::SDLK_s] = DIK_S;
		keymap[ARL::SDLK_d] = DIK_D;
		keymap[ARL::SDLK_f] = DIK_F;
		keymap[ARL::SDLK_g] = DIK_G;
		keymap[ARL::SDLK_h] = DIK_H;
		keymap[ARL::SDLK_j] = DIK_J;
		keymap[ARL::SDLK_k] = DIK_K;
		keymap[ARL::SDLK_l] = DIK_L;
		keymap[ARL::SDLK_SEMICOLON] = DIK_SEMICOLON;
		keymap[ARL::SDLK_QUOTE] = DIK_APOSTROPHE;
		keymap[ARL::SDLK_BACKQUOTE] = DIK_GRAVE;
		keymap[ARL::SDLK_LSHIFT] = DIK_LSHIFT;
		keymap[ARL::SDLK_BACKSLASH] = DIK_BACKSLASH;
		keymap[ARL::SDLK_BACKSLASH] = DIK_OEM_102;
		keymap[ARL::SDLK_z] = DIK_Z;
		keymap[ARL::SDLK_x] = DIK_X;
		keymap[ARL::SDLK_c] = DIK_C;
		keymap[ARL::SDLK_v] = DIK_V;
		keymap[ARL::SDLK_b] = DIK_B;
		keymap[ARL::SDLK_n] = DIK_N;
		keymap[ARL::SDLK_m] = DIK_M;
		keymap[ARL::SDLK_COMMA] = DIK_COMMA;
		keymap[ARL::SDLK_PERIOD] = DIK_PERIOD;
		keymap[ARL::SDLK_SLASH] = DIK_SLASH;
		keymap[ARL::SDLK_RSHIFT] = DIK_RSHIFT;
		keymap[ARL::SDLK_KP_MULTIPLY] = DIK_MULTIPLY;
		keymap[ARL::SDLK_LALT] = DIK_LMENU;
		keymap[ARL::SDLK_SPACE] = DIK_SPACE;
		keymap[ARL::SDLK_CAPSLOCK] = DIK_CAPITAL;
		keymap[ARL::SDLK_F1] = DIK_F1;
		keymap[ARL::SDLK_F2] = DIK_F2;
		keymap[ARL::SDLK_F3] = DIK_F3;
		keymap[ARL::SDLK_F4] = DIK_F4;
		keymap[ARL::SDLK_F5] = DIK_F5;
		keymap[ARL::SDLK_F6] = DIK_F6;
		keymap[ARL::SDLK_F7] = DIK_F7;
		keymap[ARL::SDLK_F8] = DIK_F8;
		keymap[ARL::SDLK_F9] = DIK_F9;
		keymap[ARL::SDLK_F10] = DIK_F10;
		keymap[ARL::SDLK_NUMLOCK] = DIK_NUMLOCK;
		keymap[ARL::SDLK_SCROLLOCK] = DIK_SCROLL;
		keymap[ARL::SDLK_KP7] = DIK_NUMPAD7;
		keymap[ARL::SDLK_KP8] = DIK_NUMPAD8;
		keymap[ARL::SDLK_KP9] = DIK_NUMPAD9;
		keymap[ARL::SDLK_KP_MINUS] = DIK_SUBTRACT;
		keymap[ARL::SDLK_KP4] = DIK_NUMPAD4;
		keymap[ARL::SDLK_KP5] = DIK_NUMPAD5;
		keymap[ARL::SDLK_KP6] = DIK_NUMPAD6;
		keymap[ARL::SDLK_KP_PLUS] = DIK_ADD;
		keymap[ARL::SDLK_KP1] = DIK_NUMPAD1;
		keymap[ARL::SDLK_KP2] = DIK_NUMPAD2;
		keymap[ARL::SDLK_KP3] = DIK_NUMPAD3;
		keymap[ARL::SDLK_KP0] = DIK_NUMPAD0;
		keymap[ARL::SDLK_KP_PERIOD] = DIK_DECIMAL;
		keymap[ARL::SDLK_F11] = DIK_F11;
		keymap[ARL::SDLK_F12] = DIK_F12;
		keymap[ARL::SDLK_F13] = DIK_F13;
		keymap[ARL::SDLK_F14] = DIK_F14;
		keymap[ARL::SDLK_F15] = DIK_F15;
		keymap[ARL::SDLK_KP_EQUALS] = DIK_NUMPADEQUALS;
		keymap[ARL::SDLK_KP_ENTER] = DIK_NUMPADENTER;
		keymap[ARL::SDLK_RCTRL] = DIK_RCONTROL;
		keymap[ARL::SDLK_KP_DIVIDE] = DIK_DIVIDE;
		keymap[ARL::SDLK_SYSREQ] = DIK_SYSRQ;
		keymap[ARL::SDLK_RALT] = DIK_RMENU;
		keymap[ARL::SDLK_PAUSE] = DIK_PAUSE;
		keymap[ARL::SDLK_HOME] = DIK_HOME;
		keymap[ARL::SDLK_UP] = DIK_UP;
		keymap[ARL::SDLK_PAGEUP] = DIK_PRIOR;
		keymap[ARL::SDLK_LEFT] = DIK_LEFT;
		keymap[ARL::SDLK_RIGHT] = DIK_RIGHT;
		keymap[ARL::SDLK_END] = DIK_END;
		keymap[ARL::SDLK_DOWN] = DIK_DOWN;
		keymap[ARL::SDLK_PAGEDOWN] = DIK_NEXT;
		keymap[ARL::SDLK_INSERT] = DIK_INSERT;
		keymap[ARL::SDLK_DELETE] = DIK_DELETE;
		keymap[ARL::SDLK_LMETA] = DIK_LWIN;
		keymap[ARL::SDLK_RMETA] = DIK_RWIN;
		keymap[ARL::SDLK_MENU] = DIK_APPS;
		initialized = true;
	}

	return keymap[keyCode];
}


// Maps ARL::ARL::SDLK_* to VK_* 
DWORD UserInputUtil::keyCodeToVK(ARL::KeyCode keyCode)
{
	static DWORD keymap[ARL::SDLK_LAST];
	static bool initialized = false;
	if (!initialized)
	{
		for ( int i=0; i<ARL::SDLK_LAST; ++i )
			keymap[i] = 0;

		keymap[ARL::SDLK_PRINT] = VK_PRINT;
		keymap[ARL::SDLK_SYSREQ] = VK_SNAPSHOT;
		keymap[ARL::SDLK_ESCAPE] = VK_ESCAPE;
		keymap[ARL::SDLK_BACKSPACE] = VK_BACK;
		keymap[ARL::SDLK_TAB] = VK_TAB;
		keymap[ARL::SDLK_RETURN] = VK_RETURN;
		keymap[ARL::SDLK_LCTRL] = VK_LCONTROL;
		keymap[ARL::SDLK_LSHIFT] = VK_LSHIFT;
		keymap[ARL::SDLK_BACKSLASH] = VK_OEM_102;
		keymap[ARL::SDLK_RSHIFT] = VK_RSHIFT;
		keymap[ARL::SDLK_KP_MULTIPLY] = VK_MULTIPLY;
		keymap[ARL::SDLK_LALT] = VK_LMENU;
		keymap[ARL::SDLK_SPACE] = VK_SPACE;
		keymap[ARL::SDLK_CAPSLOCK] = VK_CAPITAL;
		keymap[ARL::SDLK_F1] = VK_F1;
		keymap[ARL::SDLK_F2] = VK_F2;
		keymap[ARL::SDLK_F3] = VK_F3;
		keymap[ARL::SDLK_F4] = VK_F4;
		keymap[ARL::SDLK_F5] = VK_F5;
		keymap[ARL::SDLK_F6] = VK_F6;
		keymap[ARL::SDLK_F7] = VK_F7;
		keymap[ARL::SDLK_F8] = VK_F8;
		keymap[ARL::SDLK_F9] = VK_F9;
		keymap[ARL::SDLK_F10] = VK_F10;
		keymap[ARL::SDLK_NUMLOCK] = VK_NUMLOCK;
		keymap[ARL::SDLK_SCROLLOCK] = VK_SCROLL;
		keymap[ARL::SDLK_KP7] = VK_NUMPAD7;
		keymap[ARL::SDLK_KP8] = VK_NUMPAD8;
		keymap[ARL::SDLK_KP9] = VK_NUMPAD9;
		keymap[ARL::SDLK_KP_MINUS] = VK_SUBTRACT;
		keymap[ARL::SDLK_KP4] = VK_NUMPAD4;
		keymap[ARL::SDLK_KP5] = VK_NUMPAD5;
		keymap[ARL::SDLK_KP6] = VK_NUMPAD6;
		keymap[ARL::SDLK_KP_PLUS] = VK_ADD;
		keymap[ARL::SDLK_KP1] = VK_NUMPAD1;
		keymap[ARL::SDLK_KP2] = VK_NUMPAD2;
		keymap[ARL::SDLK_KP3] = VK_NUMPAD3;
		keymap[ARL::SDLK_KP0] = VK_NUMPAD0;
		keymap[ARL::SDLK_KP_PERIOD] = VK_DECIMAL;
		keymap[ARL::SDLK_F11] = VK_F11;
		keymap[ARL::SDLK_F12] = VK_F12;
		keymap[ARL::SDLK_F13] = VK_F13;
		keymap[ARL::SDLK_F14] = VK_F14;
		keymap[ARL::SDLK_F15] = VK_F15;
		keymap[ARL::SDLK_KP_ENTER] = VK_RETURN;
		keymap[ARL::SDLK_RCTRL] = VK_RCONTROL;
		keymap[ARL::SDLK_KP_DIVIDE] = VK_DIVIDE;
		keymap[ARL::SDLK_RALT] = VK_RMENU;
		keymap[ARL::SDLK_HOME] = VK_HOME;
		keymap[ARL::SDLK_UP] = VK_UP;
		keymap[ARL::SDLK_PAGEUP] = VK_PRIOR;
		keymap[ARL::SDLK_LEFT] = VK_LEFT;
		keymap[ARL::SDLK_RIGHT] = VK_RIGHT;
		keymap[ARL::SDLK_END] = VK_END;
		keymap[ARL::SDLK_DOWN] = VK_DOWN;
		keymap[ARL::SDLK_PAGEDOWN] = VK_NEXT;
		keymap[ARL::SDLK_INSERT] = VK_INSERT;
		keymap[ARL::SDLK_DELETE] = VK_DELETE;
		keymap[ARL::SDLK_LMETA] = VK_LWIN;
		keymap[ARL::SDLK_RMETA] = VK_RWIN;
		keymap[ARL::SDLK_MENU] = VK_APPS;
		initialized = true;
	}

	return keymap[keyCode];
}

ARL::InputObject::UserInputState UserInputUtil::msgToEventState(UINT uMsg)
{
	switch (uMsg)
	{
	case WM_MOUSEMOVE:
		return ARL::InputObject::INPUT_STATE_CHANGE;
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:  // intentional fall thru
		return ARL::InputObject::INPUT_STATE_BEGIN;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP: // intentional fall thru
		return ARL::InputObject::INPUT_STATE_END;
	default:
		return ARL::InputObject::INPUT_STATE_NONE;
	}
}
ARL::InputObject::UserInputType UserInputUtil::msgToEventType(UINT uMsg)
{
	switch (uMsg)
	{
		case WM_MOUSEMOVE:
			return ARL::InputObject::TYPE_MOUSEMOVEMENT;
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP: // intentional fall thru
			return ARL::InputObject::TYPE_MOUSEBUTTON1;
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP: // intentional fall thru
			return ARL::InputObject::TYPE_MOUSEBUTTON2;
		default:
			return ARL::InputObject::TYPE_NONE;
	}
}


ARL::ModCode UserInputUtil::createModCode(const DiKeys& diKeys)
{
	unsigned int modCode = 0;

	if (diKeys[DIK_LSHIFT] & 0x80)
	{
		modCode = modCode | ARL::KMOD_LSHIFT;
	} 
	if (diKeys[DIK_RSHIFT] & 0x80)
	{
		modCode = modCode | ARL::KMOD_RSHIFT;
	} 
	if (diKeys[DIK_LCONTROL] & 0x80)
	{
		modCode = modCode | ARL::KMOD_LCTRL;
	} 
	if (diKeys[DIK_RCONTROL] & 0x80)
	{
		modCode = modCode | ARL::KMOD_RCTRL;
	} 
	if (diKeys[DIK_LMENU] & 0x80)
	{
		modCode = modCode | ARL::KMOD_LALT;
	} 
	if (diKeys[DIK_LMENU] & 0x80)
	{
		modCode = modCode | ARL::KMOD_RALT;
	}
	/*if (diKeys[DIK_CAPSLOCK] & 0x80)
	{
		modCode = modCode | ARL::KMOD_CAPS;
	}*/
	if(::GetKeyState(VK_CAPITAL))
	{
		modCode = modCode | ARL::KMOD_CAPS;
	}

	return (ARL::ModCode)modCode;
}

