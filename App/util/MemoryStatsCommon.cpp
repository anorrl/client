#include "stdafx.h"

#include "util/MemoryStats.h"
#include <vector>
#include "rbx/Memory.h"

DYNAMIC_FASTINTVARIABLE(StreamingMemoryUsagePercent, 50)

#if defined(ARL_PLATFORM_IOS) || defined(__ANDROID__)
FASTINTVARIABLE(StreamingSafeMemWatermarkMB, 30)
FASTINTVARIABLE(StreamingLowMemWatermarkMB, 10)
FASTINTVARIABLE(StreamingCriticalLowMemWatermarkMB, 5)
FASTINTVARIABLE(StremingMemoryPoolReleaseThresholdMB, 2)
#else
FASTINTVARIABLE(StreamingSafeMemWatermarkMB, 100)
FASTINTVARIABLE(StreamingLowMemWatermarkMB, 30)
FASTINTVARIABLE(StreamingCriticalLowMemWatermarkMB, 15)
FASTINTVARIABLE(StremingMemoryPoolReleaseThresholdMB, 5)
#endif

namespace ARL {
extern std::vector<size_t*> poolAvailabilityList;
extern std::vector<releaseFunc> poolReleaseMemoryFuncList;
#ifdef ARL_POOL_ALLOCATION_STATS
extern std::vector<size_t*> poolAllocationList;
#endif

namespace MemoryStats {

memsize_t minTargetMemoryBytes()
{
    ARLASSERT(MemoryStats::totalMemoryBytes() > 0);
	return MemoryStats::totalMemoryBytes() * (DFInt::StreamingMemoryUsagePercent * 0.01f);
}

size_t slowGetMemoryPoolAllocation()
{
    // keep the definition here

    size_t totalPoolAllocation = 0;
#ifdef ARL_POOL_ALLOCATION_STATS
    std::vector<size_t*>::iterator iter;
    for (iter = ARL::poolAllocationList.begin(); iter != ARL::poolAllocationList.end(); iter++)
    {
        totalPoolAllocation+=**iter;
    }
#endif
    return totalPoolAllocation;
}

size_t slowGetMemoryPoolAvailability()
{
    // keep the definition here
    size_t totalPoolAvailability = 0;
    std::vector<size_t*>::iterator iter;
    for (iter = ARL::poolAvailabilityList.begin(); iter != ARL::poolAvailabilityList.end(); iter++)
    {
        totalPoolAvailability+=**iter;
    }
    return totalPoolAvailability;
}

void releaseAllPoolMemory()
{
    // keep the definition here

    std::vector<releaseFunc>::iterator iter;
    for (iter = ARL::poolReleaseMemoryFuncList.begin(); iter != ARL::poolReleaseMemoryFuncList.end(); iter++)
    {
        (*iter)();
    }
}

MemoryLevel slowCheckMemoryLevel(memsize_t extraMemoryUsed)
{
    static memsize_t kSafeMemWatermarkBytes = FInt::StreamingSafeMemWatermarkMB*1024*1024;
    static memsize_t kLowMemWatermarkBytes = FInt::StreamingLowMemWatermarkMB*1024*1024;
    static memsize_t kCriticalLowMemWatermarkBytes = FInt::StreamingCriticalLowMemWatermarkMB*1024*1024;
    static size_t kMemoryPoolReleaseThresholdBytes = FInt::StremingMemoryPoolReleaseThresholdMB*1024*1024;
	static memsize_t minMemory = minTargetMemoryBytes();
    
    // keep the definition here
    size_t totalPoolAvailability = slowGetMemoryPoolAvailability();
    memsize_t physicalMemoryAvailability = MemoryStats::freeMemoryBytes();
    
    memsize_t bytesToMinMemory = 0;
	memsize_t used = MemoryStats::usedMemoryBytes() - extraMemoryUsed;
    if (minMemory > used)
        bytesToMinMemory = minMemory - used;
    
    if ((physicalMemoryAvailability >= kSafeMemWatermarkBytes + extraMemoryUsed) || (bytesToMinMemory >= kSafeMemWatermarkBytes))
    {
        // physical memory above low water mark. No action needed.
        return MEMORYLEVEL_OK;
    }
    else if ((physicalMemoryAvailability >= kLowMemWatermarkBytes + extraMemoryUsed) || bytesToMinMemory >= kLowMemWatermarkBytes)
    {
        return MEMORYLEVEL_LIMITED; // New memory allocation should be restricted.
    }
    else if ((physicalMemoryAvailability < kCriticalLowMemWatermarkBytes + extraMemoryUsed) || bytesToMinMemory < kCriticalLowMemWatermarkBytes)
    {
        if (totalPoolAvailability >= kMemoryPoolReleaseThresholdBytes)
        {
            // only physical memory below critical low water mark. Memory pool release is needed.
            return MEMORYLEVEL_ONLY_PHYSICAL_CRITICAL_LOW;
        }
        else
        {
            // both pool and physical memory below critical low water mark. Aggressive GC is needed. New memory allocation should be restricted.
            return MEMORYLEVEL_ALL_CRITICAL_LOW;
        }
    }
    else if ((totalPoolAvailability + physicalMemoryAvailability >= kLowMemWatermarkBytes + extraMemoryUsed) || (totalPoolAvailability + bytesToMinMemory >= kLowMemWatermarkBytes))
    {
        // only physical memory below low water mark. New memory allocation should be partially restricted.
        return MEMORYLEVEL_ONLY_PHYSICAL_LOW;
    }
    else
    {
        // sum of physical memory and pool below low water mark. GC is needed to free more memory pool.
        return MEMORYLEVEL_ALL_LOW;
    }
}
}
}
