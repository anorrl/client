#include "stdafx.h"
#include "v8datamodel/TweenBase.h"
#include "v8datamodel/Tween.h"

namespace ARL
{
	REFLECTION_BEGIN();
	static Reflection::RefPropDescriptor<Tween, Instance> prop_instance(
		"Instance", category_Behavior, &Tween::getInstance, NULL, Reflection::PropertyDescriptor::STANDARD_NO_REPLICATE);
	static Reflection::PropDescriptor<Tween, TweenInfo> prop_twnInfo(
		"TweenInfo", category_Behavior, &Tween::getTweenInfo, NULL, Reflection::PropertyDescriptor::STANDARD_NO_REPLICATE);
	REFLECTION_END();

	const char* const sTween = "Tween";

	Tween::Tween(shared_ptr<Instance> instance, TweenInfo twnInfo, shared_ptr<const Reflection::ValueArray> propTable)
		: DescribedNonCreatable<Tween, TweenBase, sTween, Reflection::ClassDescriptor::RUNTIME_LOCAL>(instance, twnInfo, propTable)
		, instance(instance)
		, twnInfo(twnInfo)
	{
		setName(sTween);
	}
} // namespace ARL
