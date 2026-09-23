/* Copyright 2003-2005 ROBLOX Corporation, All Rights Reserved */

#pragma once

#include "v8tree/Instance.h"
#include "util/RunStateOwner.h"
#include "GfxBase/IAdornable.h"
#include "v8datamodel/Effect.h"

namespace ARL {
	
	class PartInstance;

	static const float largeSize = 1.1f;

	extern const char* const sForceField;
	class ForceField : public DescribedCreatable<ForceField, Instance, sForceField>
					, public IAdornable
					, public Effect
	{
	private:
		//typedef DescribedCreatable<ForceField, Instance, sForceField> Super;

		Time startTime;
		int cycle;
		int invertCycle;
		shared_ptr<Instance> torso;

		// Instance
		/*override*/ bool askSetParent(const Instance* instance) const;

		// IAdornable
		/*override*/ bool shouldRender3dAdorn() const {return true;}
		/*override*/ void render3dAdorn(Adorn* adorn);

	public:
		ForceField();
		virtual ~ForceField() {}

		static bool partInForceField(PartInstance* part);
		static int cycles() {return 60;}
	};
}	// namespace ARL
