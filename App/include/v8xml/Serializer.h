#ifndef V8XML_SERIALIZER_H
#define V8XML_SERIALIZER_H

#pragma once

#include "SerializerV2.h"

#include "util/SoundService.h"

#include "v8datamodel/Workspace.h"
#include "v8datamodel/Lighting.h"
#include "v8datamodel/ServerStorage.h"
#include "v8datamodel/ReplicatedStorage.h"
#include "v8datamodel/ReplicatedFirst.h"
#include "v8datamodel/PlayerGui.h"
#include "v8datamodel/Hopper.h"
#include "v8datamodel/StarterPlayerService.h"
#include "v8datamodel/ServerScriptService.h"
#include "v8datamodel/CSGDictionaryService.h"

class Serializer : public SerializerV2 
{
public:
	static bool canWriteChild(const shared_ptr<ARL::Instance> instance, ARL::Instance::SaveFilter saveFilter)
	{
		if(!instance->getIsArchivable())
			return false;

		switch(saveFilter)
		{
		case ARL::Instance::SAVE_ALL:
			return true;

		case ARL::Instance::SAVE_WORLD:
			if ( ARL::Instance::fastDynamicCast<ARL::Workspace>(instance.get()) )
				return true;
			if ( ARL::Instance::fastDynamicCast<ARL::Lighting>(instance.get()) )
				return true;
			if ( ARL::Instance::fastDynamicCast<ARL::Soundscape::SoundService>(instance.get()) )
				return true;
			if ( ARL::Instance::fastDynamicCast<ARL::ServerStorage>(instance.get()) )
				return true;
			if ( ARL::Instance::fastDynamicCast<ARL::ReplicatedStorage>(instance.get()) )
				return true;
			if ( ARL::Instance::fastDynamicCast<ARL::CSGDictionaryService>(instance.get()) )
				return true;

			return false;

		case ARL::Instance::SAVE_GAME:
			if ( ARL::Instance::fastDynamicCast<ARL::StarterGuiService>(instance.get()) )
				return true;
			if ( ARL::Instance::fastDynamicCast<ARL::StarterPackService>(instance.get()) )
				return true;
			if ( ARL::Instance::fastDynamicCast<ARL::StarterPlayerService>(instance.get()) )
				return true;
			if ( ARL::Instance::fastDynamicCast<ARL::ServerScriptService>(instance.get()) )
				return true;
			if ( ARL::Instance::fastDynamicCast<ARL::ReplicatedFirst>(instance.get()) )
				return true;

			return false;

		default:
			return true;
		}
	}
};



#endif