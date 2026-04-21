#pragma once

#include "Reflection/Type.h"

class XmlElement;

namespace ARL
{
	class WebSerializer {
	public:
		static XmlElement* writeTable(const ARL::Reflection::ValueMap& result);
		static XmlElement* writeList(const ARL::Reflection::ValueArray& result);
		static XmlElement* writeEntry(const std::string& key, const ARL::Reflection::Variant& value);
		static XmlElement* writeValue(const ARL::Reflection::Variant& value);
	};
}
