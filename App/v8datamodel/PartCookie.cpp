/* Copyright 2003-2005 ROBLOX Corporation, All Rights Reserved */
#include "stdafx.h"

#include "V8DataModel/PartCookie.h"
#include "v8datamodel/DataModelMesh.h"
#include "v8datamodel/Decal.h"
#include "v8datamodel/Workspace.h"
#include "v8datamodel/BasicPartInstance.h"
#include "v8datamodel/ExtrudedPartInstance.h"
#include "v8datamodel/PrismInstance.h"
#include "v8datamodel/PyramidInstance.h"


namespace ARL {

unsigned int PartCookie::compute(ARL::PartInstance* part)
{
	unsigned int cookie = part->getCookie() & IS_HUMANOID_PART;

	if (const copy_on_write_ptr<Instances>& children = part->getChildren())
	{
		for (size_t i = 0; i < children->size(); ++i)
		{
			Instance* instance = (*children)[i].get();

			// Note: all data about the special shape refers to the last special shape!
			if (Instance::fastDynamicCast<DataModelMesh>(instance))
			{
				cookie |= HAS_SPECIALSHAPE;
				cookie &= ~(HAS_FILEMESH | HAS_HEADMESH); // clear any leftover flags from the previous special shape

				if (SpecialShape* shape = Instance::fastDynamicCast<SpecialShape>(instance))
				{
					if (shape->getMeshType() == SpecialShape::FILE_MESH)
						cookie |= HAS_FILEMESH;
					else if (shape->getMeshType() == SpecialShape::HEAD_MESH)
						cookie |= HAS_HEADMESH;
				}
				else if (Instance::fastDynamicCast<FileMesh>(instance))
				{
					cookie |= HAS_FILEMESH;
				}
			}
			else if (ARL::Decal* decal = Instance::fastDynamicCast<ARL::Decal>(instance))
			{
				if (!decal->getTexture().isNull())
				{
					cookie |= HAS_DECALS;

					if (decal->getFace() == ARL::NORM_Z_NEG)
					{
						cookie |= HAS_DECALS_Z_NEG;
					}
				}
			}
		}
	}
	return cookie;
}

} // namespace