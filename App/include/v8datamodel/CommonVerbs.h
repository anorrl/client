/* Copyright 2003-2006 ROBLOX Corporation, All Rights Reserved */

#pragma once

#include "v8datamodel/DataModel.h"
#include "v8datamodel/Commands.h"
#include "v8datamodel/ToolsPart.h"
#include "v8datamodel/ToolsSurface.h"
#include "v8datamodel/ToolsModel.h"
#include "tool/ToolsArrow.h"
#include "tool/ResizeTool.h"
#include "tool/HammerTool.h"
#include "tool/GrabTool.h"
#include "tool/CloneTool.h"
#include "tool/NullTool.h"
#include "tool/GameTool.h"
#include "tool/AxisMoveTool.h"
#include "tool/AxisRotateTool.h"
#include "tool/MoveResizeJoinTool.h"
#include "tool/AdvMoveTool.h"
#include "tool/AdvRotateTool.h"
#include "v8datamodel/UndoRedo.h"
#include "v8datamodel/InputObject.h"
#include "v8tree/Instance.h"
#include "v8tree/Service.h"
#include "util/RunStateOwner.h"

#include "util/InsertMode.h"

class XmlElement;


namespace ARL {

class Fonts;
class GuiRoot;
class GuiItem;
class ContentProvider;
class TimeState;
class Hopper;
class PlayerHopper;
class StarterPackService;
class Adorn;

// Contain a set of verbs used by ANORRL
class CommonVerbs
{
public:
	CommonVerbs(DataModel* dataModel);
	///////////////////////////////////////////////////////////////////////
	//
	// Play Mode Commands
	PlayDeleteSelectionVerb playDeleteSelectionVerb;

	// Edit Menu
	DeleteSelectionVerb deleteSelectionVerb;
	SelectAllCommand selectAllCommand;
	SelectChildrenVerb selectChildrenVerb;
	SnapSelectionVerb snapSelectionVerb;
	UnlockAllVerb unlockAllVerb;
	ColorVerb colorVerb;
	MaterialVerb materialVerb;
	
	// Format Menu
	AnchorVerb anchorVerb;
	TranslucentVerb translucentVerb;
	CanCollideVerb canCollideVerb;
	CanNotSelectCommand canNotSelectCommand;
	AllCanSelectCommand allCanSelectCommand;
	MoveUpPlateVerb moveUpPlateVerb;
	MoveUpBrickVerb moveUpBrickVerb;
	MoveDownSelectionVerb moveDownSelectionVerb;
	RotateSelectionVerb rotateSelectionVerb;
	TiltSelectionVerb tiltSelectionVerb;

	// Run Menu
	RunCommand runCommand;
	StopCommand stopCommand;
	ResetCommand resetCommand;

	// Test Menu
	FirstPersonCommand firstPersonCommand;
	StatsCommand statsCommand;
	RenderStatsCommand renderStatsCommand;
	EngineStatsCommand engineStatsCommand;
	NetworkStatsCommand networkStatsCommand;
	PhysicsStatsCommand physicsStatsCommand;
	SummaryStatsCommand summaryStatsCommand;
	CustomStatsCommand customStatsCommand;
	JoinCommand joinCommand;

	// Adv Build Related
	TurnOnManualJointCreation turnOnManualJointCreationVerb;
    
	SetDragGridToOne setDragGridToOneVerb;
	SetDragGridToOneFifth setDragGridToOneFifthVerb;
	SetDragGridToOff setDragGridToOffVerb;

    SetGridSizeToTwo setGridSizeToTwoVerb;
    SetGridSizeToFour setGridSizeToFourVerb;
    SetGridSizeToSixteen setGridSizeToSixteenVerb;

	SetManualJointToWeak setManualJointToWeakVerb;
	SetManualJointToStrong setManualJointToStrongVerb;
	SetManualJointToInfinite setManualJointToInfiniteVerb;

	// Tools
	TToolVerb<AxisRotateTool> axisRotateToolVerb;
	TToolVerb<AdvMoveTool> advMoveToolVerb;
	TToolVerb<AdvRotateTool> advRotateToolVerb;
	TToolVerb<AdvArrowTool> advArrowToolVerb;
	TToolVerb<MoveResizeJoinTool> resizeToolVerb;

	TToolVerb<FlatTool> flatToolVerb;
	TToolVerb<GlueTool> glueToolVerb;
	TToolVerb<WeldTool> weldToolVerb;
	TToolVerb<StudsTool> studsToolVerb;
	TToolVerb<InletTool> inletToolVerb;
	TToolVerb<UniversalTool> universalToolVerb;
	TToolVerb<HingeTool> hingeToolVerb;
	TToolVerb<RightMotorTool> rightMotorToolVerb;
	TToolVerb<LeftMotorTool> leftMotorToolVerb;
	TToolVerb<OscillateMotorTool> oscillateMotorToolVerb;
	TToolVerb<SmoothNoOutlinesTool> smoothNoOutlinesToolVerb;

	TToolVerb<AnchorTool> anchorToolVerb;
	TToolVerb<LockTool> lockToolVerb;

	TToolVerb<FillTool> fillToolVerb;
	TToolVerb<MaterialTool> materialToolVerb;
	TToolVerb<DropperTool> dropperToolVerb;

	// Runtime Tools
	TToolVerb<GameTool> gameToolVerb;
	TToolVerb<GrabTool> grabToolVerb;
	TToolVerb<CloneTool> cloneToolVerb;
	TToolVerb<HammerTool> hammerToolVerb;
};

} // namespace
