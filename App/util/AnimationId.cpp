/* Copyright 2003-2005 ROBLOX Corporation, All Rights Reserved */
#include "stdafx.h"

#include "V8DataModel/ContentProvider.h"
#include "Util/AnimationId.h"


#include "V8DataModel/Workspace.h"
#include "V8DataModel/Camera.h"

#include "util/standardout.h"

namespace ARL {

// TODO: Refactor: It is a little ugly to have to implement these 6 functions for each "ContentID" derived class
//  Potentially we can template this a little. Maybe define a templated ContentIDPropDescriptor or something.
template<>
std::string StringConverter<AnimationId>::convertToString(const AnimationId& value)
{
	return value.toString();
}

template<>
bool StringConverter<AnimationId>::convertToValue(const std::string& text, AnimationId& value)
{
	value = text;
	return true;
}

namespace Reflection {

template<>
const Type& Type::getSingleton<ARL::AnimationId>()
{
	return Type::singleton<ContentId>();
}


template<>
void TypedPropertyDescriptor<ARL::AnimationId>::readValue(DescribedBase* instance, const XmlElement* element, IReferenceBinder& binder) const
{
	if (!element->isXsiNil())
	{
		ContentId value;
		if (element->getValue(value))
			setValue(instance, value);
	}
}

template<>
void TypedPropertyDescriptor<ARL::AnimationId>::writeValue(const DescribedBase* instance, XmlElement* element) const
{
	element->setValue(ContentId(getValue(instance)));
}



template<>
ARL::AnimationId& Variant::convert<AnimationId>(void)
{
	if (_type->isType<std::string>())
	{
		value = ARL::AnimationId(cast<std::string>());
		_type = &Type::singleton<AnimationId>();
	}
	return genericConvert<ARL::AnimationId>();
}

template<>
int TypedPropertyDescriptor<ARL::AnimationId>::getDataSize(const DescribedBase* instance) const 
{
    return sizeof(ARL::AnimationId) + getValue(instance).toString().size();
}

template<>
bool TypedPropertyDescriptor<ARL::AnimationId>::hasStringValue() const {
	return true;
}

template<>
std::string TypedPropertyDescriptor<ARL::AnimationId>::getStringValue(const DescribedBase* instance) const{
	return StringConverter<ARL::AnimationId>::convertToString(getValue(instance));
}

template<>
bool TypedPropertyDescriptor<ARL::AnimationId>::setStringValue(DescribedBase* instance, const std::string& text) const {
	ARL::AnimationId value;
	if (StringConverter<ARL::AnimationId>::convertToValue(text, value))
	{
		setValue(instance, value);
		return true;
	}
	else
		return false;
}

} // namespace Reflection
} // namespace ARL 
