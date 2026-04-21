/*
 *  RenderStatsItem.h
 *  RobloxStudio
 *  Copied from old Roblox app directory
 *
 *  Created by Vinod on 10/18/11.
 *  Copyright 2011 Roblox. All rights reserved.
 *
 */
#pragma once

#include "V8Datamodel/Stats.h"
#include "GfxBase/RenderStats.h"
#include "GfxBase/FrameRateManager.h"
#include "boost/shared_ptr.hpp"

namespace ARL
{
	namespace Stats
	{
		class Item;
		class StatsService;
	}
	namespace Profiling
	{
		class BucketProfile;
	}
}

class RenderStatsItem : public ARL::Stats::Item
{
public:
	RenderStatsItem(const ARL::RenderStats& renderStats);
	void update();

	static boost::shared_ptr<RenderStatsItem> create(const ARL::RenderStats& renderStats);
	static void addBucketToItem(ARL::Stats::Item* item, const ARL::Profiling::BucketProfile& bucketProfile);

private:
	ARL::Stats::Item* m_videoMemory;
	ARL::Stats::Item* m_majorStateChanges;
	ARL::Stats::Item* m_minorStateChanges;
	const ARL::RenderStats& m_renderStats;
};
