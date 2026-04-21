#include "stdafx.h"

#include "V8DataModel/BasicPartInstance.h"
#include "V8DataModel/ExtrudedPartInstance.h"
#include "V8DataModel/PrismInstance.h"
#include "V8DataModel/PyramidInstance.h"
#include "V8DataModel/Handles.h"
#include "V8DataModel/GuiObject.h"
#include "Reflection/EnumConverter.h"

namespace ARL {
namespace Reflection {
template<>
EnumDesc<ARL::BasicPartInstance::LegacyPartType>::EnumDesc()
:EnumDescriptor("PartType")
{
	addPair(ARL::BasicPartInstance::BALL_LEGACY_PART, "Ball");
	addPair(ARL::BasicPartInstance::BLOCK_LEGACY_PART, "Block");
	addPair(ARL::BasicPartInstance::CYLINDER_LEGACY_PART, "Cylinder");
}

template<>
EnumDesc<ARL::ExtrudedPartInstance::VisualTrussStyle>::EnumDesc()
:EnumDescriptor("Style")
{
	addPair(ARL::ExtrudedPartInstance::FULL_ALTERNATING_CROSS_BEAM, "AlternatingSupports");
	addPair(ARL::ExtrudedPartInstance::BRIDGE_STYLE_CROSS_BEAM, "BridgeStyleSupports");
	addPair(ARL::ExtrudedPartInstance::NO_CROSS_BEAM, "NoSupports");
	addLegacyName("Alternating Supports", ARL::ExtrudedPartInstance::FULL_ALTERNATING_CROSS_BEAM);
	addLegacyName("Bridge Style Supports", ARL::ExtrudedPartInstance::BRIDGE_STYLE_CROSS_BEAM);
	addLegacyName("No Supports", ARL::ExtrudedPartInstance::NO_CROSS_BEAM);
}

#ifdef _PRISM_PYRAMID_
template<>
EnumDesc<ARL::PrismInstance::NumSidesEnum>::EnumDesc()
:EnumDescriptor("PrismSides")
{
	addPair(ARL::PrismInstance::sides3, "3");
	// Don't allow a 4 sided prism - should use block.
	addPair(ARL::PrismInstance::sides5, "5");
	addPair(ARL::PrismInstance::sides6, "6");
	addPair(ARL::PrismInstance::sides8, "8");
	addPair(ARL::PrismInstance::sides10, "10");
	addPair(ARL::PrismInstance::sides20, "20");
}

template<>
EnumDesc<ARL::PyramidInstance::NumSidesEnum>::EnumDesc()
:EnumDescriptor("PyramidSides")
{
	addPair(ARL::PyramidInstance::sides3, "3");
	addPair(ARL::PyramidInstance::sides4, "4");
	addPair(ARL::PyramidInstance::sides5, "5");
	addPair(ARL::PyramidInstance::sides6, "6");
	addPair(ARL::PyramidInstance::sides8, "8");
	addPair(ARL::PyramidInstance::sides10, "10");
	addPair(ARL::PyramidInstance::sides20, "20");
}
#endif //_PRISM_PYRAMID_

template<>
EnumDesc<ARL::Handles::VisualStyle>::EnumDesc()
:EnumDescriptor("HandlesStyle")
{
	addPair(ARL::Handles::RESIZE_HANDLES, "Resize");
	addPair(ARL::Handles::MOVEMENT_HANDLES, "Movement");
}

template<>
EnumDesc<ARL::GuiObject::SizeConstraint>::EnumDesc()
:EnumDescriptor("SizeConstraint")
{
	addPair(ARL::GuiObject::RELATIVE_XY, "RelativeXY");
	addPair(ARL::GuiObject::RELATIVE_XX, "RelativeXX");
	addPair(ARL::GuiObject::RELATIVE_YY, "RelativeYY");
}

template<>
EnumDesc<ARL::GuiObject::ImageScale>::EnumDesc()
:EnumDescriptor("ScaleType")
{
	addPair(ARL::GuiObject::SCALE_STRETCH, "Stretch");
	addPair(ARL::GuiObject::SCALE_SLICED, "Slice");
}
}//namespace Reflection
}//namespace ARL
