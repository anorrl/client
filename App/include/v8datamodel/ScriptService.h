#pragma once

#include "v8tree/Instance.h"
#include "v8tree/Service.h"

namespace ARL
{
	extern const char* const sScriptService;
	class ScriptService
		: public DescribedNonCreatable<ScriptService, Instance, sScriptService>
		, public Service
	{
	};
}
