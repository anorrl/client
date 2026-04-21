/**
 * RenderStatsItem.cpp
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#include "stdafx.h"
#include "RenderStatsItem.h"
#include "rbx/SystemUtil.h"

// Roblox Headers
#include "util/Profiling.h"

RenderStatsItem::RenderStatsItem(const ARL::RenderStats& renderStats)
: m_renderStats(renderStats)
{
}

boost::shared_ptr<RenderStatsItem> RenderStatsItem::create(const ARL::RenderStats& renderStats)
{
	boost::shared_ptr<RenderStatsItem> pResult = ARL::Creatable<ARL::Instance>::create<RenderStatsItem, const ARL::RenderStats&>(renderStats);
	
	ARL::Stats::Item* pItem3D = pResult->createBoundChildItem(*(renderStats.cpuRenderTotal));
	
	pItem3D->createBoundChildItem(*(renderStats.culling));
	pItem3D->createBoundChildItem(*(renderStats.flip));
	pItem3D->createBoundChildItem(*(renderStats.renderObjects));

	pItem3D->createBoundChildItem(*(renderStats.updateLighting));
	pItem3D->createBoundChildItem(*(renderStats.adorn2D));
	pItem3D->createBoundChildItem(*(renderStats.adorn3D));
	pItem3D->createBoundChildItem(*(renderStats.visualEngineSceneUpdater));
	pItem3D->createBoundChildItem(*(renderStats.finishRendering));
	pItem3D->createBoundChildItem(*(renderStats.renderTargetUpdate));
	pItem3D->createBoundChildItem(*(renderStats.updateDynamicParts));
	pItem3D->createBoundChildItem(*(renderStats.frameRateManager));

	ARL::Stats::Item* updateScene = pItem3D->createBoundChildItem(*(renderStats.updateSceneGraph));
	updateScene->createBoundChildItem(*(renderStats.updateAllInvalidParts));
	updateScene->createBoundChildItem(*(renderStats.updateDynamicsAndAggregateStatics));
	
	ARL::Stats::Item* pMem = pResult->createChildItem("Memory");
	pResult->m_videoMemory = pMem->createChildItem("Video");

	ARL::Stats::Item* pStateChanges = pResult->createChildItem("State Changes");
	pResult->m_majorStateChanges = pStateChanges->createChildItem("Major");
	pResult->m_minorStateChanges = pStateChanges->createChildItem("Minor");

	return pResult;
}

void RenderStatsItem::update()
{
	ARL::Stats::Item::update();

    m_videoMemory->formatMem(size_t(ARL::SystemUtil::getVideoMemory()));
}

void RenderStatsItem::addBucketToItem(ARL::Stats::Item* item, const ARL::Profiling::BucketProfile& bucketProfile)
{
	const std::vector<int>& data = bucketProfile.getData();
	const int* bucketLimits = bucketProfile.getLimits();

	char temp[50];
	sprintf_s(temp, 50, "Less than %u", bucketLimits[0]);
	item->createBoundChildItem(temp, data[0]);
	for(unsigned i = 1; i < data.size()-1; i++)
	{
		sprintf_s(temp, 50, "%u to %u", bucketLimits[i-1], bucketLimits[i]);
		item->createBoundChildItem(temp, data[i]);
	}
	sprintf_s(temp, 50, "More than %u", bucketLimits[data.size()-2]);
	item->createBoundChildItem(temp, data[data.size()-1]);
}
