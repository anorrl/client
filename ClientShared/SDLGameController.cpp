#include "stdafx.h"
#include "SDLGameController.h"

#include "v8datamodel/DataModel.h"
#include "v8datamodel/GamepadService.h"
#include "v8datamodel/UserInputService.h"
#include "v8datamodel/ContentProvider.h"

#define MAX_AXIS_VALUE 32767.0f

SDLGameController::SDLGameController(shared_ptr<ARL::DataModel> newDM)
{
	dataModel = newDM;
    initSDL();
}

void SDLGameController::initSDL()
{
	if (SDL_Init(SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC) != 0)
{	
		std::string error = SDL_GetError();
		fprintf(stderr,
			"\nUnable to initialize SDL:  %s\n",
			error.c_str()
			);
		return;
	}

	ARL::ContentId gameControllerDb = ARL::ContentId::fromAssets("fonts/gamecontrollerdb.txt");
	std::string filePath = ARL::ContentProvider::findAsset(gameControllerDb);

	if (SDL_GameControllerAddMappingsFromFile(filePath.c_str()) == -1)
	{
		std::string error = SDL_GetError();
		ARL::StandardOut::singleton()->printf(ARL::MESSAGE_ERROR, "Unable to add SDL controller mappings because %s", error.c_str());
	}

	if (shared_ptr<ARL::DataModel> sharedDM = dataModel.lock())
	{
		sharedDM->submitTask(boost::bind(&SDLGameController::bindToDataModel, this), ARL::DataModelJob::Write);
	}
}

void SDLGameController::bindToDataModel()
{
	if (ARL::UserInputService* inputService = getUserInputService())
	{
		renderSteppedConnection = inputService->updateInputSignal.connect(boost::bind(&SDLGameController::updateControllers, this));
		getSupportedGamepadKeyCodesConnection = inputService->getSupportedGamepadKeyCodesSignal.connect(boost::bind(&SDLGameController::findAvailableGamepadKeyCodesAndSet, this, _1));
	}

	if (ARL::HapticService* hapticService = getHapticService())
	{
		setEnabledVibrationMotorsConnection = hapticService->setEnabledVibrationMotorsSignal.connect(boost::bind(&SDLGameController::setVibrationMotorsEnabled, this, _1));
		setVibrationMotorConnection = hapticService->setVibrationMotorSignal.connect(boost::bind(&SDLGameController::setVibrationMotor, this, _1, _2, _3));
	}
}

SDLGameController::~SDLGameController()
{
	renderSteppedConnection.disconnect();
	getSupportedGamepadKeyCodesConnection.disconnect();

	setEnabledVibrationMotorsConnection.disconnect();
	setVibrationMotorConnection.disconnect();

	for (boost::unordered_map<int, HapticData>::iterator iter = hapticsFromGamepadId.begin(); iter != hapticsFromGamepadId.end(); ++iter )
	{
		SDL_Haptic* haptic = iter->second.hapticDevice;
		int hapticEffectId = iter->second.hapticEffectId;

		SDL_HapticDestroyEffect(haptic, hapticEffectId);
		SDL_HapticClose(haptic);
	}
	hapticsFromGamepadId.clear();

    SDL_Quit();
}

ARL::UserInputService* SDLGameController::getUserInputService()
{
	if (shared_ptr<ARL::DataModel> sharedDM = dataModel.lock())
	{
		if (ARL::UserInputService* inputService = ARL::ServiceProvider::create<ARL::UserInputService>(sharedDM.get()))
		{
			return inputService;
		}
	}

	return NULL;
}

ARL::HapticService* SDLGameController::getHapticService()
{
	if (shared_ptr<ARL::DataModel> sharedDM = dataModel.lock())
	{
		if (ARL::HapticService* hapticService = ARL::ServiceProvider::create<ARL::HapticService>(sharedDM.get()))
		{
			return hapticService;
		}
	}

	return NULL;
}

ARL::GamepadService* SDLGameController::getGamepadService()
{
	if (shared_ptr<ARL::DataModel> sharedDM = dataModel.lock())
	{
		if (ARL::GamepadService* gamepadService = ARL::ServiceProvider::create<ARL::GamepadService>(sharedDM.get()))
		{
			return gamepadService;
		}
	}

	return NULL;
}

SDL_GameController* SDLGameController::removeControllerMapping(int joystickId)
{
	SDL_GameController* gameController = NULL;
	ARL::UserInputService* inputService = getUserInputService();

	if (joystickIdToGamepadId.find(joystickId) != joystickIdToGamepadId.end())
	{
		int gamepadId = joystickIdToGamepadId[joystickId];
		if (gamepadIdToGameController.find(gamepadId) != gamepadIdToGameController.end())
		{
			gameController = gamepadIdToGameController[gamepadId].second;
			gamepadIdToGameController.erase(gamepadId);

			if (inputService)
			{
                inputService->safeFireGamepadDisconnected(ARL::GamepadService::getGamepadEnumForInt(gamepadId));
			}
		}

		if (hapticsFromGamepadId.find(gamepadId) != hapticsFromGamepadId.end())
		{
			SDL_Haptic* haptic = hapticsFromGamepadId[gamepadId].hapticDevice;
			int hapticEffectId = hapticsFromGamepadId[gamepadId].hapticEffectId;

			SDL_HapticDestroyEffect(haptic, hapticEffectId);
			SDL_HapticClose(haptic);

			hapticsFromGamepadId.erase(gamepadId);
		}
	}
	
	return gameController;
}

void SDLGameController::setupControllerId(int joystickId, int gamepadId, SDL_GameController *pad)
{
	gamepadIdToGameController[gamepadId] = std::pair<int,SDL_GameController*>(joystickId, pad);
	joystickIdToGamepadId[joystickId] = gamepadId;

	if (ARL::UserInputService* inputService = getUserInputService())
	{
        inputService->safeFireGamepadConnected(ARL::GamepadService::getGamepadEnumForInt(gamepadId));
	}
}

void SDLGameController::addController(int gamepadId)
{
	if ( SDL_IsGameController(gamepadId) ) 
	{
		SDL_GameController *pad = SDL_GameControllerOpen(gamepadId);

		if (pad)
		{
			SDL_Joystick *joy = SDL_GameControllerGetJoystick(pad);
			int joystickId = SDL_JoystickInstanceID(joy);

			setupControllerId(joystickId, gamepadId, pad);
		}
	}
}

void SDLGameController::removeController(int joystickId)
{
	if (SDL_GameController *pad = removeControllerMapping(joystickId))
	{
		SDL_GameControllerClose(pad);
	}
}

ARL::Gamepad SDLGameController::getRbxGamepadFromJoystickId(int joystickId)
{
	if (joystickIdToGamepadId.find(joystickId) != joystickIdToGamepadId.end())
	{
		if (ARL::GamepadService* gamepadService = getGamepadService())
		{
			int gamepadId = joystickIdToGamepadId[joystickId];
			return gamepadService->getGamepadState(gamepadId);
		}
	}

	return ARL::Gamepad();
}

ARL::KeyCode getKeyCodeFromSDLAxis(SDL_GameControllerAxis sdlAxis, int& axisValueChanged)
{
	switch (sdlAxis)
	{
	case SDL_CONTROLLER_AXIS_LEFTX:
		axisValueChanged = 0;
		return ARL::SDLK_GAMEPAD_THUMBSTICK1;
	case SDL_CONTROLLER_AXIS_LEFTY:
		axisValueChanged = 1;
		return ARL::SDLK_GAMEPAD_THUMBSTICK1;
	case SDL_CONTROLLER_AXIS_RIGHTX:
		axisValueChanged = 0;
		return ARL::SDLK_GAMEPAD_THUMBSTICK2;
	case SDL_CONTROLLER_AXIS_RIGHTY:
		axisValueChanged = 1;
		return ARL::SDLK_GAMEPAD_THUMBSTICK2;
	case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
		axisValueChanged = 2;
		return ARL::SDLK_GAMEPAD_BUTTONL2;
	case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
		axisValueChanged = 2;
		return ARL::SDLK_GAMEPAD_BUTTONR2;
    case SDL_CONTROLLER_AXIS_INVALID:
    case SDL_CONTROLLER_AXIS_MAX:
        return ARL::SDLK_UNKNOWN;
	}

	return ARL::SDLK_UNKNOWN;
}

ARL::KeyCode getKeyCodeFromSDLButton(SDL_GameControllerButton sdlButton)
{
	switch (sdlButton)
	{
	case SDL_CONTROLLER_BUTTON_A:
		return ARL::SDLK_GAMEPAD_BUTTONA;
	case SDL_CONTROLLER_BUTTON_B:
		return ARL::SDLK_GAMEPAD_BUTTONB;
	case SDL_CONTROLLER_BUTTON_X:
		return ARL::SDLK_GAMEPAD_BUTTONX;
	case SDL_CONTROLLER_BUTTON_Y:
		return ARL::SDLK_GAMEPAD_BUTTONY;

	case SDL_CONTROLLER_BUTTON_START:
		return ARL::SDLK_GAMEPAD_BUTTONSTART;
	case SDL_CONTROLLER_BUTTON_BACK:
		return ARL::SDLK_GAMEPAD_BUTTONSELECT;

	case SDL_CONTROLLER_BUTTON_DPAD_UP:
		return ARL::SDLK_GAMEPAD_DPADUP;
	case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
		return ARL::SDLK_GAMEPAD_DPADDOWN;
	case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
		return ARL::SDLK_GAMEPAD_DPADLEFT;
	case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
		return ARL::SDLK_GAMEPAD_DPADRIGHT;

	case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
		return ARL::SDLK_GAMEPAD_BUTTONL1;
	case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
		return ARL::SDLK_GAMEPAD_BUTTONR1;

	case SDL_CONTROLLER_BUTTON_LEFTSTICK:
		return ARL::SDLK_GAMEPAD_BUTTONL3;
	case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
		return ARL::SDLK_GAMEPAD_BUTTONR3;
            
    case SDL_CONTROLLER_BUTTON_INVALID:
    case SDL_CONTROLLER_BUTTON_GUIDE:
    case SDL_CONTROLLER_BUTTON_MAX:
        return ARL::SDLK_UNKNOWN;
	}

	return ARL::SDLK_UNKNOWN;
}

bool SDLGameController::setupHapticsForDevice(int id)
{
	// already set up
	if (hapticsFromGamepadId.find(id) != hapticsFromGamepadId.end())
	{
		return true;
	}

	SDL_Haptic *haptic = NULL;

	// Open the device
	haptic = SDL_HapticOpen(id);
	if (haptic)
	{
		HapticData hapticData;
		hapticData.hapticDevice = haptic;
		hapticData.hapticEffectId = -1;
		hapticData.currentLeftMotorValue = 0.0f;
		hapticData.currentRightMotorValue = 0.0f;

		hapticsFromGamepadId[id] = hapticData;

		return true;
	}

	return false;
}

void SDLGameController::setVibrationMotorsEnabled(ARL::InputObject::UserInputType gamepadType)
{
	int gamepadId = getGamepadIntForEnum(gamepadType);
	if (!setupHapticsForDevice(gamepadId))
	{
		return;
	}

	SDL_Haptic* haptic = hapticsFromGamepadId[gamepadId].hapticDevice;
	if (haptic)
	{
		if (ARL::HapticService* hapticService = getHapticService())
		{
			hapticService->setEnabledVibrationMotors(gamepadType, ARL::HapticService::MOTOR_LARGE, true);
			hapticService->setEnabledVibrationMotors(gamepadType, ARL::HapticService::MOTOR_SMALL, true);
			hapticService->setEnabledVibrationMotors(gamepadType, ARL::HapticService::MOTOR_LEFTTRIGGER, false);
			hapticService->setEnabledVibrationMotors(gamepadType, ARL::HapticService::MOTOR_RIGHTTRIGGER, false);
		}
	}
}

void SDLGameController::setVibrationMotor(ARL::InputObject::UserInputType gamepadType, ARL::HapticService::VibrationMotor vibrationMotor, shared_ptr<const ARL::Reflection::Tuple> args)
{
	int gamepadId = getGamepadIntForEnum(gamepadType);
	if (!setupHapticsForDevice(gamepadId))
	{
		return;
	}

	float newMotorValue = 0.0f;
	ARL::Reflection::Variant newValue = args->values[0];
	if (newValue.isFloat())
	{
		newMotorValue = newValue.get<float>();
		newMotorValue = G3D::clamp(newMotorValue, 0.0f, 1.0f);
	}
	else // no valid number in first position, lets bail
	{
		ARL::StandardOut::singleton()->printf(ARL::MESSAGE_ERROR, "First value to HapticService:SetMotor is not a valid number (must be a number between 0-1)");
		return;
	}

	boost::unordered_map<int, HapticData>::iterator iter = hapticsFromGamepadId.find(gamepadId);

	// make sure we grab old data so we set the motors that haven't changed value
	float leftMotorValue = iter->second.currentLeftMotorValue;
	float rightMotorValue = iter->second.currentRightMotorValue;

	if (vibrationMotor == ARL::HapticService::MOTOR_LARGE)
	{
		leftMotorValue = newMotorValue;
	}
	else if (vibrationMotor == ARL::HapticService::MOTOR_SMALL)
	{
		rightMotorValue = newMotorValue;
	}

	SDL_Haptic* haptic = iter->second.hapticDevice;
	int oldEffectId = iter->second.hapticEffectId;
	if (oldEffectId >= 0)
	{
		SDL_HapticDestroyEffect(haptic, oldEffectId);
	}

	if (leftMotorValue <= 0.0f && rightMotorValue <= 0.0f)
	{
		HapticData hapticData;
		hapticData.hapticDevice = haptic;
		hapticData.hapticEffectId = -1;
		hapticData.currentLeftMotorValue = 0.0f;
		hapticData.currentRightMotorValue = 0.0f;

		hapticsFromGamepadId[gamepadId] = hapticData;
		return;
	}

	// Create the left/right effect
	SDL_HapticEffect effect;
	memset( &effect, 0, sizeof(SDL_HapticEffect) ); // 0 is safe default
	effect.type = SDL_HAPTIC_LEFTRIGHT;
	effect.leftright.large_magnitude = 65535.0f * leftMotorValue;
	effect.leftright.small_magnitude = 65535.0f * rightMotorValue;
	effect.leftright.length = SDL_HAPTIC_INFINITY;

	// Upload the effect
	int hapticEffectId = SDL_HapticNewEffect(haptic, &effect);

	HapticData hapticData;
	hapticData.hapticDevice = haptic;
	hapticData.hapticEffectId = hapticEffectId;
	hapticData.currentLeftMotorValue = leftMotorValue;
	hapticData.currentRightMotorValue = rightMotorValue;

	hapticsFromGamepadId[gamepadId] = hapticData;

	if (haptic && hapticEffectId >= 0)
	{
		SDL_HapticRunEffect(haptic, hapticEffectId, SDL_HAPTIC_INFINITY);
	}
}

void SDLGameController::refreshHapticEffects()
{
	for (boost::unordered_map<int, HapticData>::iterator iter = hapticsFromGamepadId.begin(); iter != hapticsFromGamepadId.end(); ++iter )
	{
		SDL_Haptic* haptic = iter->second.hapticDevice;
		int hapticEffectId = iter->second.hapticEffectId;

		if (haptic && hapticEffectId >= 0)
		{
			SDL_HapticRunEffect(haptic, hapticEffectId, SDL_HAPTIC_INFINITY);
		}
	}
}

void SDLGameController::onControllerButton( const SDL_ControllerButtonEvent sdlEvent )
{
	const ARL::KeyCode buttonCode = getKeyCodeFromSDLButton((SDL_GameControllerButton) sdlEvent.button);

	if (buttonCode == ARL::SDLK_UNKNOWN)
	{
		return;
	}

	ARL::Gamepad gamepad = getRbxGamepadFromJoystickId(sdlEvent.which);
	const int buttonState = (sdlEvent.type == SDL_CONTROLLERBUTTONDOWN) ? 1 : 0;
	ARL::InputObject::UserInputState newState = (buttonState == 1) ? ARL::InputObject::INPUT_STATE_BEGIN : ARL::InputObject::INPUT_STATE_END;

	if (newState == gamepad[buttonCode]->getUserInputState())
	{
		return;
	}

	const G3D::Vector3 lastPos = gamepad[buttonCode]->getPosition();

	gamepad[buttonCode]->setPosition(G3D::Vector3(0,0,buttonState));
	gamepad[buttonCode]->setDelta(gamepad[buttonCode]->getPosition() - lastPos);
	gamepad[buttonCode]->setInputState(newState);

	if (ARL::UserInputService* inputService = getUserInputService())
	{
        inputService->dangerousFireInputEvent(gamepad[buttonCode], NULL);
	}
}

void SDLGameController::onControllerAxis( const SDL_ControllerAxisEvent sdlEvent )
{
	int axisValueChanged = -1;
	const ARL::KeyCode axisCode = getKeyCodeFromSDLAxis((SDL_GameControllerAxis) sdlEvent.axis, axisValueChanged);

	if (axisCode == ARL::SDLK_UNKNOWN)
	{
		return;
	}

	float axisValue = sdlEvent.value;
	axisValue /= MAX_AXIS_VALUE;
	axisValue = G3D::clamp(axisValue, -1.0f, 1.0f);

	ARL::Gamepad gamepad = getRbxGamepadFromJoystickId(sdlEvent.which);
	G3D::Vector3 currentPosition = gamepad[axisCode]->getPosition();

	switch (axisValueChanged)
	{
	case 0:
		currentPosition.x = axisValue;
		break;
	case 1:
		currentPosition.y = -axisValue;
		break;
	case 2:
		currentPosition.z = axisValue;
		break;
	default:
		break;
	}

	G3D::Vector3 lastPos = gamepad[axisCode]->getPosition();
	if (lastPos != currentPosition)
	{
		gamepad[axisCode]->setPosition(currentPosition);

		ARL::InputObject::UserInputState currentState = ARL::InputObject::INPUT_STATE_CHANGE;
		if (currentPosition == G3D::Vector3::zero())
		{
			currentState = ARL::InputObject::INPUT_STATE_END;
		}
		else if (currentPosition.z >= 1.0f)
		{
			currentState = ARL::InputObject::INPUT_STATE_BEGIN;
		}

		gamepad[axisCode]->setDelta(currentPosition - lastPos);
		gamepad[axisCode]->setInputState(currentState);

		if (ARL::UserInputService* inputService = getUserInputService())
		{
            inputService->dangerousFireInputEvent(gamepad[axisCode], NULL);
		}
	}
}

void SDLGameController::updateControllers()
{
	SDL_Event sdlEvent;

	while( SDL_PollEvent( &sdlEvent ) ) 
	{
		switch( sdlEvent.type ) 
		{
		case SDL_CONTROLLERDEVICEADDED:
			addController( sdlEvent.cdevice.which );
			break;

		case SDL_CONTROLLERDEVICEREMOVED:
			removeController( sdlEvent.cdevice.which );
			break;

		case SDL_CONTROLLERBUTTONDOWN:
		case SDL_CONTROLLERBUTTONUP:
			onControllerButton( sdlEvent.cbutton );
			break;

		case SDL_CONTROLLERAXISMOTION:
			onControllerAxis( sdlEvent.caxis );
			break;

		default:
			break;
		}
	}

	refreshHapticEffects();
}

ARL::KeyCode getKeyCodeFromSDLName(std::string sdlName)
{
	if (sdlName.compare("a") == 0)
		return ARL::SDLK_GAMEPAD_BUTTONA;
	if (sdlName.compare("b") == 0)
		return ARL::SDLK_GAMEPAD_BUTTONB;
	if (sdlName.compare("x") == 0)
		return ARL::SDLK_GAMEPAD_BUTTONX;
	if (sdlName.compare("y") == 0)
		return ARL::SDLK_GAMEPAD_BUTTONY;

	if (sdlName.compare("back") == 0)
		return ARL::SDLK_GAMEPAD_BUTTONSELECT;
	if (sdlName.compare("start") == 0)
		return ARL::SDLK_GAMEPAD_BUTTONSTART;

	if (sdlName.compare("dpdown") == 0)
		return ARL::SDLK_GAMEPAD_DPADDOWN;
	if (sdlName.compare("dpleft") == 0)
		return ARL::SDLK_GAMEPAD_DPADLEFT;
	if (sdlName.compare("dpright") == 0)
		return ARL::SDLK_GAMEPAD_DPADRIGHT;
	if (sdlName.compare("dpup") == 0)
		return ARL::SDLK_GAMEPAD_DPADUP;

	if (sdlName.compare("leftshoulder") == 0)
		return ARL::SDLK_GAMEPAD_BUTTONL1;
	if (sdlName.compare("lefttrigger") == 0)
		return ARL::SDLK_GAMEPAD_BUTTONL2;
	if (sdlName.compare("leftstick") == 0)
		return ARL::SDLK_GAMEPAD_BUTTONL3;

	if (sdlName.compare("rightshoulder") == 0)
		return ARL::SDLK_GAMEPAD_BUTTONR1;
	if (sdlName.compare("righttrigger") == 0)
		return ARL::SDLK_GAMEPAD_BUTTONR2;
	if (sdlName.compare("rightstick") == 0)
		return ARL::SDLK_GAMEPAD_BUTTONR3;

	if (sdlName.compare("leftx") == 0 ||
		sdlName.compare("lefty") == 0)
		return ARL::SDLK_GAMEPAD_THUMBSTICK1;

	if (sdlName.compare("rightx") == 0 ||
		sdlName.compare("righty") == 0)
		return ARL::SDLK_GAMEPAD_THUMBSTICK2;

	return ARL::SDLK_UNKNOWN;
}

int SDLGameController::getGamepadIntForEnum(ARL::InputObject::UserInputType gamepadType)
{
	switch (gamepadType)
	{
	case ARL::InputObject::TYPE_GAMEPAD1:
		return 0;
	case ARL::InputObject::TYPE_GAMEPAD2:
		return 1;
	case ARL::InputObject::TYPE_GAMEPAD3:
		return 2;
	case ARL::InputObject::TYPE_GAMEPAD4:
		return 3;
	default:
		break;
	}

	return -1;
}

void SDLGameController::findAvailableGamepadKeyCodesAndSet(ARL::InputObject::UserInputType gamepadType)
{
	shared_ptr<const ARL::Reflection::ValueArray> availableGamepadKeyCodes = getAvailableGamepadKeyCodes(gamepadType);
	if (ARL::UserInputService* inputService = getUserInputService())
	{
		inputService->setSupportedGamepadKeyCodes(gamepadType, availableGamepadKeyCodes);
	}
}

shared_ptr<const ARL::Reflection::ValueArray> SDLGameController::getAvailableGamepadKeyCodes(ARL::InputObject::UserInputType gamepadType)
{
	int gamepadId = getGamepadIntForEnum(gamepadType);

	if ( gamepadId < 0 || (gamepadIdToGameController.find(gamepadId) == gamepadIdToGameController.end()) )
	{
		return shared_ptr<const ARL::Reflection::ValueArray>();
	}

	if (SDL_GameController* gameController = gamepadIdToGameController[gamepadId].second)
	{
		std::string gameControllerMapping(SDL_GameControllerMapping(gameController));

		std::istringstream controllerMappingStream(gameControllerMapping);
		std::string mappingItem;

		shared_ptr<ARL::Reflection::ValueArray> supportedGamepadFunctions(new ARL::Reflection::ValueArray());

		int count = 0;
		while(std::getline(controllerMappingStream, mappingItem, ',')) 
		{
			// first two settings in mapping are hardware id and device name, don't need those
			if (count > 1)
			{
				std::istringstream mappingStream(mappingItem);
				std::string sdlName;
				std::getline(mappingStream, sdlName, ':');

				// platform is always last thing defined in mappings, don't need it so we are done
				if (sdlName.compare("platform") == 0)
				{
					break;
				}

				ARL::KeyCode gamepadCode = getKeyCodeFromSDLName(sdlName);
				if (gamepadCode != ARL::SDLK_UNKNOWN)
				{
					supportedGamepadFunctions->push_back(gamepadCode);
				}
			}
			count++;
		}

		return supportedGamepadFunctions;
	}

	return shared_ptr<const ARL::Reflection::ValueArray>();
}
