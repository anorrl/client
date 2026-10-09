#include "stdafx.h"

#include "util/Region3int32.h"
#include "util/ExtentsInt32.h"
#include "reflection/Type.h"
namespace ARL {

	namespace Reflection
	{
		template<>
		const Type& Type::getSingleton<Region3int32>()
		{
			static TType<Region3int32> type("Region3int32");
			return type;
		}
	}
	Region3int32::Region3int32()
	{}

	Region3int32::Region3int32(const Vector3int32& min, const Vector3int32& max)
	{	
		minPos = min;
		maxPos = max;
	}

	Vector3int32 Region3int32::getMinPos() const
	{
		return minPos;
	}

	Vector3int32 Region3int32::getMaxPos() const
	{
		return maxPos;
	}

}