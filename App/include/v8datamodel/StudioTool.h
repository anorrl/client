/* Copyright 2003-2007 ROBLOX Corporation, All Rights Reserved */

#pragma once
#include "V8Tree/Instance.h"
#include "V8Tree/Verb.h"

namespace ARL {
	class Workspace;
	class Mouse;

	extern const char *const sStudioTool;

	class StudioTool 
		: public DescribedNonCreatable<StudioTool, Instance, sStudioTool>
	{
	protected:
		shared_ptr<Mouse> onEquipping(Workspace* workspace);
		bool enabled;
	public:
		StudioTool();

		bool getEnabled() const { return enabled; }
		void setEnabled(bool);

		void activate();
		void deactivate();

		void equip(Workspace*);
		void unequip();

		arl::signal<void(shared_ptr<Instance>)> equippedSignal;
		arl::signal<void()> activatedSignal;
		arl::signal<void()> unequippedSignal;
		arl::signal<void()> deactivatedSignal;
	};
}
