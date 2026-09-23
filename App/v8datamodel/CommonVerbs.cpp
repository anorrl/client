/* Copyright 2003-2005 ROBLOX Corporation, All Rights Reserved */
#include "stdafx.h"

#include "v8datamodel/commonverbs.h"		// TODO - minimize these includes, and in the .h file
#include "v8datamodel/Workspace.h"
#include "v8datamodel/Camera.h"
#include "v8datamodel/UserController.h"
#include "v8datamodel/JointsService.h"
#include "v8datamodel/GuiBuilder.h"
#include "v8datamodel/Sky.h"
#include "v8datamodel/Lighting.h"
#include "v8datamodel/Teams.h"
#include "v8datamodel/Hopper.h"
#include "v8datamodel/Backpack.h"
#include "v8datamodel/Tool.h"

#include "humanoid/Humanoid.h"
#include "network/Player.h"
#include "network/Players.h"

#include "v8tree/verb.h"
#include "v8world/World.h"
#include "v8kernel/Kernel.h"
#include "gui/GUI.h"
#include "script/luainstancebridge.h"
#include "script/scriptcontext.h"
#include "util/SoundWorld.h"
#include "util/Sound.h"
#include "util/StandardOut.h"
#include "arl/Log.h"
#include "appdraw/DrawPrimitives.h"
#include "util/http.h"

#include "g3d/format.h"

#include "v8xml/Serializer.h"
#include "boost/cast.hpp"
#include "boost/scoped_ptr.hpp"



namespace ARL {

CommonVerbs::CommonVerbs(DataModel* dataModel) 
	: 
	joinCommand(dataModel),
	statsCommand(dataModel),
	renderStatsCommand(dataModel),
	engineStatsCommand(dataModel),
	networkStatsCommand(dataModel),
	physicsStatsCommand(dataModel),
	summaryStatsCommand(dataModel),
	customStatsCommand(dataModel),
	runCommand(dataModel),
	stopCommand(dataModel),
	resetCommand(dataModel),
	axisRotateToolVerb(dataModel),
	resizeToolVerb(dataModel),
	advMoveToolVerb(dataModel),
	advRotateToolVerb(dataModel),
	advArrowToolVerb(dataModel, false),
	turnOnManualJointCreationVerb(dataModel),	
	setDragGridToOneVerb(dataModel),
	setDragGridToOneFifthVerb(dataModel),
	setDragGridToOffVerb(dataModel),
	setGridSizeToTwoVerb(dataModel),
	setGridSizeToFourVerb(dataModel),
	setGridSizeToSixteenVerb(dataModel),
	setManualJointToWeakVerb(dataModel),
	setManualJointToStrongVerb(dataModel),
	setManualJointToInfiniteVerb(dataModel),

	flatToolVerb(dataModel),
	glueToolVerb(dataModel),
	weldToolVerb(dataModel),
	studsToolVerb(dataModel),
	inletToolVerb(dataModel),
	universalToolVerb(dataModel),
	hingeToolVerb(dataModel),
	rightMotorToolVerb(dataModel),
	leftMotorToolVerb(dataModel),
	oscillateMotorToolVerb(dataModel),
	smoothNoOutlinesToolVerb(dataModel),

	anchorToolVerb(dataModel),
	lockToolVerb(dataModel),
	fillToolVerb(dataModel, false),
	materialToolVerb(dataModel, false),
	materialVerb(dataModel),
	colorVerb(dataModel),
	anchorVerb(dataModel),
	dropperToolVerb(dataModel),

	firstPersonCommand(dataModel),
	selectChildrenVerb(dataModel),
	snapSelectionVerb(dataModel),
	playDeleteSelectionVerb(dataModel),
	deleteSelectionVerb(dataModel),
	moveUpPlateVerb(dataModel),
	moveUpBrickVerb(dataModel),
	moveDownSelectionVerb(dataModel),
	rotateSelectionVerb(dataModel),
	tiltSelectionVerb(dataModel),
	selectAllCommand(dataModel),
	allCanSelectCommand(dataModel),
	canNotSelectCommand(dataModel),
	translucentVerb(dataModel),
	canCollideVerb(dataModel),
	unlockAllVerb(dataModel),
	gameToolVerb(dataModel),
	grabToolVerb(dataModel),
	cloneToolVerb(dataModel),
	hammerToolVerb(dataModel)
{
}


}
