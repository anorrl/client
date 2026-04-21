/* Copyright 2003-2005 ROBLOX Corporation, All Rights Reserved */
#include "stdafx.h"

#include "V8DataModel/ContentProvider.h"
#include "Util/MeshId.h"


#include "V8DataModel/Workspace.h"
#include "V8DataModel/Camera.h"

#include "util/standardout.h"

namespace ARL
{
	// TODO: Refactor: It is a little ugly to have to implement these 6 functions for each "ContentID" derived class
	//  Potentially we can template this a little. Maybe define a templated ContentIDPropDescriptor or something.
	template<>
	std::string StringConverter<MeshId>::convertToString(const MeshId& value)
	{
		return value.toString();
	}

	template<>
	bool StringConverter<MeshId>::convertToValue(const std::string& text, MeshId& value)
	{
		value = text;
		return true;
	}

	namespace Reflection
	{
		template<>
		const Reflection::Type& Reflection::Type::getSingleton<MeshId>()
		{
			return Reflection::Type::singleton<ContentId>();
		}


		template<>
		void TypedPropertyDescriptor<ARL::MeshId>::readValue(DescribedBase* instance, const XmlElement* element, IReferenceBinder& binder) const
		{
			if (!element->isXsiNil())
			{
				ContentId value;
				if (element->getValue(value))
					setValue(instance, value);
			}
		}

		template<>
		void TypedPropertyDescriptor<ARL::MeshId>::writeValue(const DescribedBase* instance, XmlElement* element) const
		{
			element->setValue(ContentId(getValue(instance)));
		}



		template<>
		ARL::MeshId& Variant::convert<MeshId>(void)
		{
			if (_type->isType<std::string>())
			{
				value = ARL::MeshId(cast<std::string>());
				_type = &Type::singleton<MeshId>();
			}
			return genericConvert<ARL::MeshId>();
		}

        template<>
        int TypedPropertyDescriptor<ARL::MeshId>::getDataSize(const DescribedBase* instance) const 
        {
            return sizeof(ARL::MeshId) + getValue(instance).toString().size();
        }

		template<>
		bool TypedPropertyDescriptor<ARL::MeshId>::hasStringValue() const {
			return true;
		}

		template<>
		std::string TypedPropertyDescriptor<ARL::MeshId>::getStringValue(const DescribedBase* instance) const{
			return StringConverter<ARL::MeshId>::convertToString(getValue(instance));
		}

		template<>
		bool TypedPropertyDescriptor<ARL::MeshId>::setStringValue(DescribedBase* instance, const std::string& text) const {
			ARL::MeshId value;
			if (StringConverter<ARL::MeshId>::convertToValue(text, value))
			{
				setValue(instance, value);
				return true;
			}
			else
				return false;
		}

	} // namespace Reflection
}// namespace ARL
