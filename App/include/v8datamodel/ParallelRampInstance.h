/* Copyright 2003-2009 ROBLOX Corporation, All Rights Reserved */

#pragma once

#include "v8datamodel/PartInstance.h"
#include "v8datamodel/BasicPartInstance.h"
#include "reflection/Reflection.h"

#ifdef _PRISM_PYRAMID_

namespace ARL {

extern const char* const sParallelRamp;

class ParallelRampInstance
	: public DescribedNonCreatable<ParallelRampInstance, PartInstance, sParallelRamp>
{
	public:

		ParallelRampInstance();
		~ParallelRampInstance();
	 
		/*override*/ virtual PartType getPartType() const { return PARALLELRAMP_PART; }


};

} // namespace

#endif // _PRISM_PYRAMID_