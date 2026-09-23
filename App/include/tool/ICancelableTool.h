/* Copyright 2003-2005 ROBLOX Corporation, All Rights Reserved */

#pragma once

#include "v8datamodel/MouseCommand.h"

namespace ARL
{
	class ICancelableTool
	{

	public:
		virtual shared_ptr<MouseCommand> onCancelOperation() = 0;

	};


} //namespace