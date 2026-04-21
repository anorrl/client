/* Copyright 2003-2007 ROBLOX Corporation, All Rights Reserved */

#pragma once

#include "BevelMesh.h"

namespace ARL
{
	extern const char* const sBlockMesh;
	class BlockMesh 
		: public DescribedCreatable<BlockMesh, BevelMesh, sBlockMesh>
	{
	public:
		BlockMesh(){}
	};
}