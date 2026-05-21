#include "stdafx.h"

#include "V8DataModel/GameSettings.h"
#include "V8DataModel/GameBasicSettings.h"

namespace ARL {
	namespace Reflection {
		template<>
		EnumDesc<GameSettings::UploadSetting>::EnumDesc()
		:EnumDescriptor("UploadSetting")
		{
			addPair(GameSettings::NEVER, "Never");
			addPair(GameSettings::ASK, "Ask");
			addLegacyName("Ask me first", GameSettings::ASK);
			addPair(GameSettings::ALWAYS, "Always");
		}
	}//namespace Reflection
}//namespace ARL

using namespace ARL;
const char *const ARL::sGameSettings = "GameSettings";

REFLECTION_BEGIN();
Reflection::BoundProp<int> prop_ChatHistory("ChatHistory", "Online", &GameSettings::chatHistory);
Reflection::BoundProp<int> prop_ReportAbuseChatHistory("ReportAbuseChatHistory", "Online", &GameSettings::reportAbuseChatHistory);
Reflection::BoundProp<int> prop_ChatScrollLength("ChatScrollLength", "Online", &GameSettings::chatScrollLength);
Reflection::BoundProp<bool> prop_SoundEnabled("SoundEnabled", "Sound", &GameSettings::soundEnabled);
Reflection::BoundProp<bool> prop_SoftwareSound("SoftwareSound", "Sound", &GameSettings::softwareSound);
Reflection::BoundProp<bool> prop_CollisionSoundEnabled("CollisionSoundEnabled", "Sound", &GameSettings::collisionSoundEnabled, Reflection::PropertyDescriptor::Attributes::deprecated());
Reflection::BoundProp<float> prop_CollisionSoundVolume("CollisionSoundVolume", "Sound", &GameSettings::collisionSoundVolume, Reflection::PropertyDescriptor::Attributes::deprecated());
Reflection::BoundProp<int> prop_MaxCollisionSounds("MaxCollisionSounds", "Sound", &GameSettings::maxCollisionSounds, Reflection::PropertyDescriptor::Attributes::deprecated());
Reflection::BoundProp<int> prop_bubbleChatMaxBubbles("BubbleChatMaxBubbles", "Online", &GameSettings::bubbleChatMaxBubbles);
Reflection::BoundProp<float> prop_bubbleChatLifetime("BubbleChatLifetime", "Online", &GameSettings::bubbleChatLifetime);
Reflection::BoundProp<bool> prop_hardwareMouse("HardwareMouse", "Input", &GameSettings::hardwareMouse);
REFLECTION_END();

GameSettings::GameSettings(void)
	:chatHistory(100)
	,reportAbuseChatHistory(50)
	,chatScrollLength(5)
	,soundEnabled(true)
	,collisionSoundEnabled(true)
	,collisionSoundVolume(10)
	,maxCollisionSounds(-1)
	,softwareSound(false)
	,bubbleChatMaxBubbles(3)
	,bubbleChatLifetime(30.0f)
	,hardwareMouse(false)
    ,overscanPX(-1)
    ,overscanPY(-1)
{
	setName("Game Options");
}

GameSettings::UploadSetting GameSettings::getPostImageSetting() const
{
	return GameBasicSettings::singleton().getPostImageSetting();
}

void GameSettings::setPostImageSetting(GameSettings::UploadSetting setting) 
{
	GameBasicSettings::singleton().setPostImageSetting(setting);
}