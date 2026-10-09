/* Copyright 2003-2005 ROBLOX Corporation, All Rights Reserved */

#pragma once

#include "v8tree/Service.h"
#include "util/RunStateOwner.h"
#include "reflection/Event.h"


namespace ARL {

	class PartInstance;

	class MeshId : public ContentId
	{
	public:
		MeshId(const ContentId& id):ContentId(id) {}
		MeshId(const char* id):ContentId(id) {}
		MeshId(const std::string& id):ContentId(id) {}
		MeshId() {}
	};
} // namespace ARL
