/* Copyright 2003-2009 ROBLOX Corporation, All Rights Reserved */

#pragma once

#include "v8datamodel/Mouse.h"
#include "v8datamodel/Workspace.h"
namespace ARL {

	extern const char *const sPlayerMouse;

	class PlayerMouse : public DescribedNonCreatable<PlayerMouse, Mouse, sPlayerMouse>
	{
	private:
		typedef DescribedNonCreatable<PlayerMouse, Mouse, sPlayerMouse> Super;
	public:

		PlayerMouse();
		~PlayerMouse();

		TextureId getIcon() const;
		void setIcon(const TextureId& value);
	};

} // namespace
