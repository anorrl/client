/* Copyright 2003-2014 ROBLOX Corporation, All Rights Reserved */
#include "stdafx.h"

#include "Util/Sound.h"
#include "Util/SoundService.h"

#include "V8datamodel/ContentProvider.h"
#include "V8datamodel/GameSettings.h"

#include "StringConv.h"

#include "rbx/Profiler.h"

#include "fmod.h"
#include "fmod.hpp"
#include "fmod_errors.h"

#if FMOD_VERSION != 0x00010702
#	error Wrong version of fmod.
#endif

// This is equivalent to 500 kB by default
DYNAMIC_FASTINTVARIABLE(MinSoundStreamSizeBytes, 512000)

LOGGROUP(Sound)
DYNAMIC_LOGGROUP(SoundTrace)

namespace ARL 
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// SoundId Reflection Implementation
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<>
	std::string StringConverter<Soundscape::SoundId>::convertToString(const Soundscape::SoundId& value)
	{
		return value.toString();
	}

	template<>
	bool StringConverter<Soundscape::SoundId>::convertToValue(const std::string& text, Soundscape::SoundId& value)
	{
		value = text;
		return true;
	}

	namespace Reflection {
		template<>
		const Type& Type::getSingleton<ARL::Soundscape::SoundId>()
		{
			return Type::singleton<ARL::ContentId>();
		}

		template<>
		ARL::Soundscape::SoundId& Variant::convert<ARL::Soundscape::SoundId>(void)
		{
			if (_type->isType<std::string>())
			{
				value = ARL::Soundscape::SoundId(cast<std::string>());
				_type = &Type::singleton<ARL::Soundscape::SoundId>();
			}
			return genericConvert<ARL::Soundscape::SoundId>();
		}


		template<>
		void TypedPropertyDescriptor<ARL::Soundscape::SoundId>::readValue(DescribedBase* instance, const XmlElement* element, IReferenceBinder& binder) const
		{
			if (!element->isXsiNil())
			{
				ContentId value;
				if (element->getValue(value))
					setValue(instance, value);
			}
		}

		template<>
		void TypedPropertyDescriptor<ARL::Soundscape::SoundId>::writeValue(const DescribedBase* instance, XmlElement* element) const
		{
			ContentId id(getValue(instance));
			element->setValue(id);
		}

		template<>
		int TypedPropertyDescriptor<ARL::Soundscape::SoundId>::getDataSize(const DescribedBase* instance) const 
		{
			return sizeof(ARL::Soundscape::SoundId) + getValue(instance).toString().size();
		}

		template<>
		bool TypedPropertyDescriptor<ARL::Soundscape::SoundId>::hasStringValue() const {
			return true;
		}

		template<>
		std::string TypedPropertyDescriptor<ARL::Soundscape::SoundId>::getStringValue(const DescribedBase* instance) const{
			return StringConverter<ARL::Soundscape::SoundId>::convertToString(getValue(instance));
		}

		template<>
		bool TypedPropertyDescriptor<ARL::Soundscape::SoundId>::setStringValue(DescribedBase* instance, const std::string& text) const {
			ARL::Soundscape::SoundId value;
			if (StringConverter<ARL::Soundscape::SoundId>::convertToValue(text, value))
			{
				setValue(instance, value);
				return true;
			}
			else
				return false;
		}
	}// namespace Reflection

	namespace Soundscape
    {
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Sound Object Implementation
		//
		///////////////////////////////////////////////////////////////////////////////////////////////////////////

		std::ifstream::pos_type getFilesize(const char* filename)
		{
			std::ifstream is(utf8_decode(filename).c_str(), std::ios_base::in | std::ios_base::binary);
			is.seekg (0, std::ios::end);
			return is.tellg();
		}

		// both ogg and mp3 containers allow mostly arbitrary data before a sync word.
		// People were uploading audio as a model/plugin.  This requires "<roblox" 
		static bool isFileModel(const char* filename)
		{
			std::ifstream is(utf8_decode(filename).c_str(), std::ios_base::in | std::ios_base::binary);
			char buf[7];
			is.read(buf,7);
			return (0 == strncmp(buf, "<roblox", 7));
		}

		static bool isMemModel(const std::string* data)
		{
			if (data->size() > 7)
			{
				return (0 == strncmp(data->c_str(), "<roblox", 7));
			}
			return false;
		}

		FMOD::Sound* Sound::tryLoad(const ARL::Instance* context) // throw on failure, returns null if not available yet
		{
			FASTLOG1(DFLog::SoundTrace, "Sound::get(%p)", this);

			if (fmod_sound)
                return fmod_sound;
            
            try
			{
                ARLPROFILER_SCOPE("Sound", "Sound::get");

				FMOD_MODE mode = is3D ? FMOD_3D : FMOD_DEFAULT;

                std::string fileName;
                
                {
                    ARLPROFILER_SCOPE("Sound", "getFile");

                    fileName = ServiceProvider::create<ContentProvider>(context)->getFile(id);
                }
                
				int fileSize = getFilesize(fileName.c_str());

                ARLPROFILER_LABELF("Sound", "%s (%d bytes)", id.c_str(), fileSize);

				if ( !fileName.empty() && (fileSize >= DFInt::MinSoundStreamSizeBytes) )
				{
					if (isFileModel(fileName.c_str()))
					{
						throw std::runtime_error("sound format invalid");
					}
					mode |= FMOD_CREATESTREAM;
					isStreaming = true;
                    
                    ARLPROFILER_SCOPE("Sound", "createStream");

					SoundService::checkResult(system->createStream(fileName.c_str(), mode, NULL, &fmod_sound), "createStream", this, system.get());
				}
				else
				{
					shared_ptr<const std::string> data = ServiceProvider::create<ContentProvider>(context)->requestContentString(id, ContentProvider::PRIORITY_SOUND);

					if (!data)
					{
						return NULL;
					}

					if (isMemModel(data.get()))
					{
						throw std::runtime_error("sound format invalid");
					}

					mode |= FMOD_OPENMEMORY;
					// Open-from-memory:
					FMOD_CREATESOUNDEXINFO info;
					memset(&info, 0, sizeof(FMOD_CREATESOUNDEXINFO));
					info.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
					info.length = data->size();

                    ARLPROFILER_SCOPE("Sound", "createSound");

					SoundService::checkResult(system->createSound(data->c_str(), mode, &info, &fmod_sound), "createSound", this, system.get());
				}
			}
            catch (ARL::base_exception& e)
            {
                StandardOut::singleton()->printf(MESSAGE_ERROR, "Sound failed to load %s : %s because %s", context ? context->getFullName().c_str() : "", id.c_str(), e.what());
            }

			return fmod_sound;
		}

		void Sound::release()
		{
			FASTLOG1(DFLog::SoundTrace, "Sound::release(%p)", this);
			if (fmod_sound)
            {
                    SoundService::checkResultNoThrow(fmod_sound->release(), "release", this, fmod_sound);
				fmod_sound = 0;
			}
		}
	}// namespace Soundscape
}// namespace ARL
