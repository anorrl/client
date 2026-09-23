#include "stdafx.h"

#include "util/KeywordFilter.h"
#include "reflection/EnumConverter.h"

namespace ARL 
{
	namespace Reflection
	{
		template<>
		Reflection::EnumDesc<KeywordFilterType>::EnumDesc()
			:ARL::Reflection::EnumDescriptor("KeywordFilterType")
		{
			addPair(INCLUDE_KEYWORDS,"Include");
			addPair(EXCLUDE_KEYWORDS,"Exclude");
		}
	}
}
