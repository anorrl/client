/* Copyright 2003-2007 ROBLOX Corporation, All Rights Reserved */

#pragma once

#include "v8datamodel/Hopper.h"
#include "script/IScriptFilter.h"

namespace ARL {

	extern const char *const sBackpack;
	class Backpack 
		: public DescribedCreatable<Backpack, Hopper, sBackpack>
		, public IScriptFilter
	{
	private:
		// IScriptOwner
		/*override*/ bool scriptShouldRun(BaseScript* script);

	public:
		Backpack();
	};


}	// namespace 