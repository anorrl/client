#pragma once

#include "Util/G3DCore.h"
#include "Util/ExtentsInt32.h"
#include "Util/Extents.h"
#include "rbx/Debug.h"

//#define _ARL_DEBUGGING_SPATIAL_HASH 

#ifdef _ARL_DEBUGGING_SPATIAL_HASH
	#define ARLASSERT_SPATIAL_HASH(expr) ARLASSERT(expr)
	const bool assertingSpatialHash = true;
#else
	#define ARLASSERT_SPATIAL_HASH(expr) ((void)0)
	const bool assertingSpatialHash = false;
#endif

namespace ARL {

	/** use the SpatialHash with classes that contain these members:
	 * basic Primitive must implement:
	 */ 
	class BasicSpatialHashPrimitive
	{
	private:
		ExtentsInt32			oldSpatialExtents;
		int						spatialNodeLevel;

#ifdef _ARL_DEBUGGING_SPATIAL_HASH
		void*					spatialNodes;		
		int                     spatialNodeCount;
#endif

	public:
		BasicSpatialHashPrimitive()
			: spatialNodeLevel(-1)
#ifdef _ARL_DEBUGGING_SPATIAL_HASH
			, spatialNodes(0)
			, spatialNodeCount(0)
#endif
		{};

		~BasicSpatialHashPrimitive()
		{
			ARLASSERT(spatialNodeLevel == -1);	//
			ARLASSERT_SPATIAL_HASH(spatialNodes == NULL);
			ARLASSERT_SPATIAL_HASH(spatialNodeCount == 0);
			spatialNodeLevel = -2;
		}

		bool IsInSpatialHash() { return spatialNodeLevel > -1;}

		// The remaining functions are used by the SpatialHash<> implementation
        int getSpatialNodeLevel() const {
			ARLASSERT(spatialNodeLevel >= -1);
			return spatialNodeLevel;
		}
		void setSpatialNodeLevel(int value)						{spatialNodeLevel = value;}

		const ExtentsInt32&	getOldSpatialExtents() const			{return oldSpatialExtents;}
		void setOldSpatialExtents(const ExtentsInt32& value)		{oldSpatialExtents = value;}	

		const Vector3int32& getOldSpatialMin()	{return oldSpatialExtents.low;}
		const Vector3int32& getOldSpatialMax()	{return oldSpatialExtents.high;}

	};

}	// namespace
