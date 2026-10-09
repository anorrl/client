#include "stdafx.h"

#include "util/Region3int16.h"
#include "util/ExtentsInt32.h"
#include "reflection/Type.h"
namespace ARL {
	
namespace Reflection
{
	template<>
	const Type& Type::getSingleton<Region3int16>()
	{
		static TType<Region3int16> type("Region3int16");
		return type;
	}
}

}