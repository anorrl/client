#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>
#include <boost/weak_ptr.hpp>

#include "SDL.h"
#include "SDL_gamecontroller.h"

#include "util/KeyCode.h"
#include "v8datamodel/InputObject.h"
#include "v8datamodel/HapticService.h"

namespace ARL
{
	class DataModel;
	class UserInputService;

	class GamepadService;

	typedef boost::unordered_map<ARL::KeyCode, boost::shared_ptr<ARL::InputObject> > Gamepad;
}

struct HapticData
{
	int hapticEffectId;
	float currentLeftMotorValue;
	float currentRightMotorValue;
	SDL_Haptic* hapticDevice;
};

class SDLGameController
{
private:
	boost::weak_ptr<ARL::DataModel> dataModel;
	boost::unordered_map<int, std::pair<int,SDL_GameController*> > gamepadIdToGameController;
	boost::unordered_map<int, HapticData> hapticsFromGamepadId;
	boost::unordered_map<int, int> joystickIdToGamepadId;

	rbx::signals::scoped_connection renderSteppedConnection;
	rbx::signals::scoped_connection getSupportedGamepadKeyCodesConnection;

	rbx::signals::scoped_connection setEnabledVibrationMotorsConnection;
	rbx::signals::scoped_connection setVibrationMotorConnection;

	void initSDL();

	ARL::UserInputService* getUserInputService();
	ARL::HapticService* getHapticService();

	ARL::GamepadService* getGamepadService();
	ARL::Gamepad getRbxGamepadFromJoystickId(int joystickId);

	void setupControllerId(int joystickId, int gamepadId, SDL_GameController *pad);
	SDL_GameController* removeControllerMapping(int joystickId);

	int getGamepadIntForEnum(ARL::InputObject::UserInputType gamepadType);

	void findAvailableGamepadKeyCodesAndSet(ARL::InputObject::UserInputType gamepadType);
	boost::shared_ptr<const ARL::Reflection::ValueArray> getAvailableGamepadKeyCodes(ARL::InputObject::UserInputType gamepadType);

	void bindToDataModel();

	// Haptic Functions
	void refreshHapticEffects();
	bool setupHapticsForDevice(int id);

	void setVibrationMotorsEnabled(ARL::InputObject::UserInputType gamepadType);
	void setVibrationMotor(ARL::InputObject::UserInputType gamepadType, ARL::HapticService::VibrationMotor vibrationMotor, shared_ptr<const ARL::Reflection::Tuple> args);

public:
	SDLGameController(boost::shared_ptr<ARL::DataModel> newDM);
	~SDLGameController();

	void updateControllers();

	void onControllerAxis( const SDL_ControllerAxisEvent sdlEvent );
	void onControllerButton( const SDL_ControllerButtonEvent sdlEvent );
	void removeController(int joystickId);
	void addController(int gamepadId);
};
