/* Copyright 2003-2005 ROBLOX Corporation, All Rights Reserved */
#include "stdafx.h"

#include "tool/AdvDragTool.h"
#include "v8datamodel/PartInstance.h"
#include "tool/PartDragTool.h"
#include "tool/GroupDragTool.h"
#include "tool/AdvLuaDragTool.h"

namespace ARL {


shared_ptr<MouseCommand> AdvDragTool::onMouseDown(PartInstance* hitPart,
									const Vector3& hitWorld,
									const std::vector<Instance*>& dragInstances,
									const shared_ptr<InputObject>& inputObject, 
									Workspace* workspace,
									shared_ptr<Instance> selectIfNoDrag)
{
	ARLASSERT(hitPart);

	PartArray partArray;
	DragUtilities::instancesToParts(dragInstances, partArray);

	shared_ptr<MouseCommand> advLuaDragTool = Creatable<MouseCommand>::create<AdvLuaDragTool>( hitPart, 
												hitWorld, 
												partArray, 
												workspace,
												selectIfNoDrag);
	return advLuaDragTool->onMouseDown(inputObject);
}


} // namespace
