/* Copyright 2003-2005 ROBLOX Corporation, All Rights Reserved */
#include "stdafx.h"

#include "tool/HammerTool.h"
#include "v8datamodel/Workspace.h"
#include "v8datamodel/PartInstance.h"
#include "v8datamodel/Explosion.h"
#include "v8world/Primitive.h"
#include "util/SoundService.h"
#include "util/SoundWorld.h"
#include "SelectState.h"

namespace ARL {

	
//////////////////////////////////////////////////////////////////////////////

const char* const sHammerTool = "Hammer";

HammerTool::HammerTool(Workspace* workspace)
	: Named<MouseCommand, sHammerTool>(workspace)
{
	FASTLOG1(FLog::MouseCommandLifetime, "HammerTool created: %p", this);
}

HammerTool::~HammerTool()
{
	FASTLOG1(FLog::MouseCommandLifetime, "HammerTool destroyed: %p", this);
}


void HammerTool::onMouseIdle(const shared_ptr<InputObject>& inputObject)
{
	hammerPart = shared_from(getUnlockedPartByLocalCharacter(inputObject));
}


shared_ptr<MouseCommand> HammerTool::onMouseDown(const shared_ptr<InputObject>& inputObject)
{
	if (hammerPart) {
		shared_ptr<Explosion> explosion = Creatable<Instance>::create<Explosion>();
		Explosion::propPosition.setValue(explosion.get(), hammerPart->getCoordinateFrame().translation);
		explosion->setVisualOnly();
		explosion->setParent(workspace);

		hammerPart->setParent(NULL);
		ServiceProvider::create<Soundscape::SoundService>(workspace)->playSound(BOMB_SOUND);
	}
	return shared_ptr<MouseCommand>();
}

const std::string HammerTool::getCursorName() const
{
	return hammerPart ? "HammerOverCursor" : "HammerCursor";
}


void HammerTool::render3dAdorn(Adorn* adorn)
{
	Super::render3dAdorn(adorn);

	if (hammerPart) {
		hammerPart->render3dSelect(adorn, SELECT_LIMIT);
	}
}

} // Namespace