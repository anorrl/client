#pragma once

#include "v8datamodel/PartInstance.h"
#include "v8datamodel/IAnimatableJoint.h"

namespace ARL
{
	class AnimatableRootJoint : public IAnimatableJoint
	{
		bool isAnimating;
		shared_ptr<PartInstance> part;
		CoordinateFrame lastCFrame;
	public:
		AnimatableRootJoint(const shared_ptr<PartInstance>& part);
		PartInstance* getPart() const { return part.get(); }

		/*override*/ void setAnimating(bool value);
		/*override*/ const std::string& getParentName();
		/*override*/ const std::string& getPartName();
		/*override*/ void applyPose(const CachedPose& pose);
	};
}