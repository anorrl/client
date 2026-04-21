#pragma once

#include "Reflection/Property.h"
#include "V8Xml/Reference.h"
#include "V8Xml/XmlElement.h"
#include "Util/Object.h"

#include "G3D/Vector3.h"
#include "G3D/Color3.h"
#include <string>
#include <iostream>
#include <vector>

namespace ARL {

	class PropertyChanged
	{
		ARL::Reflection::Property property;
	public:
		const ARL::Reflection::Property& getProperty() const { return property; }
		const ARL::Reflection::PropertyDescriptor& getDescriptor() const { return property.getDescriptor(); }
		const ARL::Name& getName() { return property.getName(); }

		PropertyChanged(const PropertyChanged& other)
			: property(other.property)
		{}

	private:
		friend class Instance;
		PropertyChanged(const ARL::Reflection::Property& p) 
			: property(p)
		{}
	};

// TODO: Move this out of Reflection?
// Some standard categories
#define category_Data       "Data"
#define category_Behavior   "Behavior"
#define category_State		"State"
#define category_Appearance "Appearance"
#define category_Team		"Team"
#define category_Image      "Image"
#define category_Video		"Video"
#define category_Control	"Control"

}
