/* Copyright 2003-2009 ROBLOX Corporation, All Rights Reserved */
#include "stdafx.h"

#include "v8datamodel/SurfaceSelection.h"
#include "v8datamodel/PartInstance.h"
#include "appdraw/DrawAdorn.h"
#include "appdraw/Draw.h"

namespace ARL {

const char* const sSurfaceSelection = "SurfaceSelection";

REFLECTION_BEGIN();
static const Reflection::EnumPropDescriptor<SurfaceSelection, NormalId>	prop_Surface("TargetSurface", category_Data, &SurfaceSelection::getSurface, &SurfaceSelection::setSurface);
REFLECTION_END();

SurfaceSelection::SurfaceSelection()
	:DescribedCreatable<SurfaceSelection, PartAdornment, sSurfaceSelection>("SurfaceSelection")
	,surfaceId(NORM_X)
{}

void SurfaceSelection::setSurface(NormalId value)
{
	if(surfaceId != value){
		surfaceId = value;
		raisePropertyChanged(prop_Surface);
	}
}

void SurfaceSelection::render3dAdorn(Adorn* adorn)
{
	if(getVisible()){
		if(shared_ptr<ARL::PartInstance> partInstance = adornee.lock()){
			DrawAdorn::partSurface(partInstance->getPart(), surfaceId, adorn, color);
		}
	}
}

}

// Randomized Locations for hackflags
namespace ARL 
{ 
    namespace Security
    {
        unsigned int hackFlag0 = 0;
    };
};
