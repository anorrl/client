/* Copyright 2003-2006 ROBLOX Corporation, All Rights Reserved */


#include "GfxBase/RenderStats.h"
#include "Util/Profiling.h"

using namespace ARL;

RenderStats::RenderStats() :
	cpuRenderTotal(new ARL::Profiling::CodeProfiler("3D CPU Total"))

	, culling(new ARL::Profiling::CodeProfiler("Culling"))
	, flip(new ARL::Profiling::CodeProfiler("Flipping Backbuffer"))
	, renderObjects(new ARL::Profiling::CodeProfiler("Render Objects"))
	, updateLighting(new ARL::Profiling::CodeProfiler("Update Lighting"))
	, adorn2D(new ARL::Profiling::CodeProfiler("Adorn 2D"))
	, adorn3D(new ARL::Profiling::CodeProfiler("Adorn 3D"))
	, visualEngineSceneUpdater(new ARL::Profiling::CodeProfiler("Visual Engine Scene Updater"))
   	, finishRendering(new ARL::Profiling::CodeProfiler("Finish Rendering"))
	, renderTargetUpdate(new ARL::Profiling::CodeProfiler("RenderTarget Update"))

	, frameRateManager(new ARL::Profiling::CodeProfiler("Frame Rate Manager"))

	, textureCompositor(new ARL::Profiling::CodeProfiler("Texture Compositor"))
	, updateSceneGraph(new ARL::Profiling::CodeProfiler("Update SceneGraph"))
	, updateAllInvalidParts(new ARL::Profiling::CodeProfiler("updateAllInvalidParts"))
	, updateDynamicsAndAggregateStatics(new ARL::Profiling::CodeProfiler("updateDynamicsAndAggregateStatics"))
	, updateDynamicParts(new ARL::Profiling::CodeProfiler("updateDynamicParts"))
{
	
}

RenderStats::~RenderStats()
{
}