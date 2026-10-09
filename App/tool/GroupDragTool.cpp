/* Copyright 2003-2005 ROBLOX Corporation, All Rights Reserved */
#include "stdafx.h"

#include "tool/GroupDragTool.h"
#include "tool/MegaDragger.h"
#include "tool/Dragger.h"
#include "tool/ToolsArrow.h"
#include "v8datamodel/PartInstance.h"
#include "v8datamodel/Workspace.h"
#include "v8datamodel/InputObject.h"
#include "v8datamodel/ChangeHistory.h"

namespace ARL {

const char* const sGroupDragTool = "GroupDragTool";

GroupDragTool::GroupDragTool(	PartInstance* mousePart,
								const Vector3& hitPointWorld,
								const PartArray& partArray,
								Workspace* workspace)
	: Named<MouseCommand, sGroupDragTool>(workspace)
	, dragging(false)
{
	FASTLOG1(FLog::MouseCommandLifetime, "GroupDragTool created: %p", this);
	ARLASSERT(mousePart);
	megaDragger.reset(new MegaDragger(	mousePart, 
										partArray, 
										workspace));
}


shared_ptr<MouseCommand> GroupDragTool::onMouseDown(const shared_ptr<InputObject>& inputObject)
{
	downPoint = inputObject->get2DPosition();
	capture();
	return shared_from(this);
}

void GroupDragTool::onMouseMove(const shared_ptr<InputObject>& inputObject)
{
	ARLASSERT(this->captured());

	if (!dragging) {
		if ((inputObject->get2DPosition() - downPoint).length() > 4) {
			dragging = true;
			megaDragger->startDragging();
			megaDragger->alignAndCleanParts();
			lastHit = megaDragger->hitObjectOrPlane(inputObject);
		}
	}

	onMouseIdle(inputObject);
}


void GroupDragTool::onMouseIdle(const shared_ptr<InputObject>& inputObject)
{
	ARLASSERT(captured());

	Vector3 hit;

	if (	captured() 
		&&	dragging 
		&&	megaDragger->mousePartAlive()
		&&	Math::intersectRayPlane(getSearchRay(inputObject),
									Plane(Vector3::unitY(), Vector3::zero()), 
									hit))
	{
		hit = megaDragger->hitObjectOrPlane(inputObject);

		megaDragger->continueDragging();			// do this every time - multiplayer

		Vector3 hitSnapped = DragUtilities::toGrid(hit);

		Vector3 delta = DragUtilities::toGrid(hitSnapped - lastHit);

		lastHit = hitSnapped;
		
		if (delta != Vector3::zero())
		{
			megaDragger->safeMoveYDrop(delta);
			megaDragger->safeMoveToMinimumHeight(hit.y);
		}
	}
}

shared_ptr<MouseCommand> GroupDragTool::onMouseUp(const shared_ptr<InputObject>& inputObject)
{
	ARLASSERT(this->captured());

	if (dragging) {
		megaDragger->finishDragging();
		dragging = false;
		ChangeHistoryService::requestWaypoint(getName().c_str(), workspace);
	}
	releaseCapture();
	return shared_ptr<MouseCommand>();
}


shared_ptr<MouseCommand> GroupDragTool::onKeyDown(const shared_ptr<InputObject>& inputObject)
{
	if (dragging) {
		switch (inputObject->getKeyCode())
		{
		case SDLK_r:	
			{
				megaDragger->safeRotate(Math::matrixRotateY());
				break;
			}
		case SDLK_t:
			{
				megaDragger->safeRotate(Math::matrixTiltZ());
				break;
			}
        default:
            break;
		}
		return shared_from(this);
	}
	else {
		ARLASSERT(0);		// when did this happen
		return shared_ptr<MouseCommand>();
	}
}


GroupDragTool::~GroupDragTool()
{
	if (dragging) {
		ARLASSERT(0);		// shouldn't ever get here....
	}
	FASTLOG1(FLog::MouseCommandLifetime, "GroupDragTool destroyed: %p", this);
}

} // namespace
