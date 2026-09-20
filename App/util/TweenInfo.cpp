#include "stdafx.h"
#include "V8DataModel/DataModel.h"
#include "Util/TweenInfo.h"

namespace ARL
{
	const char* const sTweenInfo = "TweenInfo";

	namespace Reflection
	{

		template<>
		EnumDesc<TweenInfo::TweenEasingDirection>::EnumDesc()
			: EnumDescriptor("EasingDirection")
		{
			addPair(TweenInfo::EASING_DIRECTION_IN, "In");
			addPair(TweenInfo::EASING_DIRECTION_OUT, "Out");
			addPair(TweenInfo::EASING_DIRECTION_IN_OUT, "InOut");
		}
		template<>
		TweenInfo::TweenEasingDirection& Variant::convert<TweenInfo::TweenEasingDirection>(void)
		{
			return genericConvert<TweenInfo::TweenEasingDirection>();
		}

		template<>
		EnumDesc<TweenInfo::TweenEasingStyle>::EnumDesc()
			: EnumDescriptor("EasingStyle")
		{
			addPair(TweenInfo::EASING_STYLE_LINEAR, "Linear");
			addPair(TweenInfo::EASING_STYLE_SINE, "Sine");
			addPair(TweenInfo::EASING_STYLE_BACK, "Back");
			addPair(TweenInfo::EASING_STYLE_QUAD, "Quad");
			addPair(TweenInfo::EASING_STYLE_QUART, "Quart");
			addPair(TweenInfo::EASING_STYLE_QUINT, "Quint");
			addPair(TweenInfo::EASING_STYLE_BOUNCE, "Bounce");
			addPair(TweenInfo::EASING_STYLE_ELASTIC, "Elastic");
			addPair(TweenInfo::EASING_STYLE_EXPONENTIAL, "Exponential");
			addPair(TweenInfo::EASING_STYLE_CIRCULAR, "Circular");
			addPair(TweenInfo::EASING_STYLE_CUBIC, "Cubic");
		}
		template<>
		TweenInfo::TweenEasingStyle& Variant::convert<TweenInfo::TweenEasingStyle>(void)
		{
			return genericConvert<TweenInfo::TweenEasingStyle>();
		}

		template<>
		EnumDesc<TweenInfo::TweenStatus>::EnumDesc()
			: EnumDescriptor("TweenStatus")
		{
			addPair(TweenInfo::TWEEN_CANCELED, "Canceled");
			addPair(TweenInfo::TWEEN_COMPLETED, "Completed");
		}
		template<>
		TweenInfo::TweenStatus& Variant::convert<TweenInfo::TweenStatus>(void)
		{
			return genericConvert<TweenInfo::TweenStatus>();
		}
	} // namespace Reflection
	template<>
	bool StringConverter<TweenInfo::TweenEasingStyle>::convertToValue(const std::string& text, TweenInfo::TweenEasingStyle& value)
	{
		return Reflection::EnumDesc<TweenInfo::TweenEasingStyle>::singleton().convertToValue(text.c_str(), value);
	}

	template<>
	bool ARL::StringConverter<TweenInfo::TweenEasingDirection>::convertToValue(const std::string& text, TweenInfo::TweenEasingDirection& value)
	{
		return Reflection::EnumDesc<TweenInfo::TweenEasingDirection>::singleton().convertToValue(text.c_str(), value);
	}


	template<>
	bool ARL::StringConverter<TweenInfo::TweenStatus>::convertToValue(const std::string& text, TweenInfo::TweenStatus& value)
	{
		return Reflection::EnumDesc<TweenInfo::TweenStatus>::singleton().convertToValue(text.c_str(), value);
	}

	size_t TweenInfo::hashCode() const
	{
		size_t seed = 0;
		boost::hash_combine(seed, time);
		boost::hash_combine(seed, style);
		boost::hash_combine(seed, direction);
		boost::hash_combine(seed, repeatCount);
		boost::hash_combine(seed, reverses);
		boost::hash_combine(seed, delayTime);
		return seed;
	}

	size_t hash_value(const TweenInfo& properties)
	{
		return properties.hashCode();
	}
} // namespace ARL