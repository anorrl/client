/* Copyright 2003-2007 ROBLOX Corporation, All Rights Reserved */
#include "stdafx.h"

#include "v8datamodel/MouseCommand.h"
#include "v8datamodel/Workspace.h"
#include "v8datamodel/PartInstance.h"
#include "v8datamodel/Camera.h"
#include "v8datamodel/Filters.h"
#include "humanoid/Humanoid.h"
#include "v8world/Primitive.h"
#include "v8world/ContactManager.h"
#include "v8world/World.h"

namespace ARL {

Vector3 MouseCommand::ignoreVector3;
bool MouseCommand::advArrowToolEnabled = false;//should be false by default

MouseCommand::MouseCommand(Workspace* workspace) :
	workspace(workspace),
	capturedMouse(false),
	rightMouseClickPart(),
	lastHoverPart()
{
}

MouseCommand::~MouseCommand()
{
}

Instance* MouseCommand::getTopSelectable3d(PartInstance* part) const
{
	ARLASSERT(part);

	// do not allow selection of a part that is not selectable
	if( !part->isSelectable3d() ) return NULL;

	Instance* answer = part;
	Instance* temp = part->getParent();
	while ( temp && temp != workspace )
	{
		Selectable* selectable = dynamic_cast<Selectable*>(temp);
		answer = selectable && selectable->isSelectable3d() ?
			temp : answer;
		temp = temp->getParent();
	}
	return answer;
}

TextureId MouseCommand::getCursorId() const
{
	std::string asset = "Textures/" + getCursorName() + ".png";
	if (getCursorName() == "StudsCursor" ||
		getCursorName() == "InletCursor" ||
		getCursorName() == "UniversalCursor") {
		asset = "Textures/FlatCursor.png";
	}
	return ContentId::fromAssets(asset.c_str());
}

void MouseCommand::capture() 
{
	ARLASSERT(!workspace->getCurrentMouseCommand()->captured());
	capturedMouse = true;
}

float MouseCommand::distanceToCharacter(const Vector3& hitPoint) const
{
	if (const PartInstance* head = Humanoid::getConstLocalHeadFromContext(workspace)) {
		Vector3 delta = head->getCoordinateFrame().translation - hitPoint;
		return delta.magnitude();
	}
	return 0;
}

bool MouseCommand::characterCanReach(const Vector3& hitPoint) const
{
	return (Humanoid::getConstLocalHeadFromContext(workspace) && distanceToCharacter(hitPoint) < 60.0);
}

Surface MouseCommand::getSurface(Workspace* workspace, const shared_ptr<InputObject>& inputObject, const HitTestFilter* filter,
								   PartInstance*& part, 
								   int& surfaceId)
{
	if ((part = getPart(workspace, inputObject, filter))) {
		ARL::RbxRay gridRay = getUnitMouseRay(inputObject, workspace);
		return part->getSurface(gridRay, surfaceId);
	}
	return Surface();
}


Surface MouseCommand::getSurface(Workspace* workspace, const shared_ptr<InputObject>& inputObject, const HitTestFilter* filter)
{
	PartInstance* part;
	int surfaceId;
	return getSurface(workspace, inputObject, filter, part, surfaceId);
}

RbxRay MouseCommand::getUnitMouseRay(const shared_ptr<InputObject>& inputObject, ICameraOwner* _workspace)
{
	return _workspace->getConstCamera()->worldRay( inputObject->getRawPosition().x, inputObject->getRawPosition().y);
}

RbxRay MouseCommand::getSearchRay(const RbxRay& unitRay)
{
	ARLASSERT(unitRay.direction().isUnit());

	return RbxRay::fromOriginAndDirection(unitRay.origin(), unitRay.direction() * maxSearch());
}


RbxRay MouseCommand::getSearchRay(const shared_ptr<InputObject>& inputObject, ICameraOwner* _workspace)
{
	RbxRay unitRay = getUnitMouseRay(inputObject, _workspace);

	return getSearchRay(unitRay);
}

RbxRay MouseCommand::getUnitMouseRay(const shared_ptr<InputObject>& inputObject) const 
{
	return getUnitMouseRay(inputObject, workspace);
}

RbxRay MouseCommand::getSearchRay(const shared_ptr<InputObject>& inputObject) const	
{
	return getSearchRay(inputObject, workspace);
}


///////////////////////////////////////////////////////////////////////////////

PartInstance* MouseCommand::getPartByLocalCharacter(Workspace* workspace, const shared_ptr<InputObject>& inputObject, const HitTestFilter* filter, Vector3& hitWorld)
{
    PartByLocalCharacter characterFilter(workspace);
    MergedFilter mergedFilter(&characterFilter, filter);
    return getPart(workspace, inputObject, &mergedFilter, hitWorld);
}

///////////////////////////////////////////////////////////////////////////////

PartInstance* MouseCommand::getUnlockedPartByLocalCharacter(const shared_ptr<InputObject>& inputObject, Vector3& hitWorld)
{
	UnlockedPartByLocalCharacter filter(workspace);
	return getPart(workspace, inputObject, &filter, hitWorld);
}

///////////////////////////////////////////////////////////////////////////////

PartInstance* MouseCommand::getUnlockedPart(const shared_ptr<InputObject>& inputObject, Vector3& hitWorld)
{
	PartInstance* part = getPart(workspace, inputObject, NULL, hitWorld);
	if(part && !part->getPartLocked())
		return part;
	else
		return NULL;
}

///////////////////////////////////////////////////////////////////////////////

PartInstance* MouseCommand::getPart(
	Workspace* workspace,
	const shared_ptr<InputObject>& inputObject, 
	const HitTestFilter* filter,
	Vector3& hitWorld)
{
	RbxRay unitRay = getUnitMouseRay(inputObject, workspace);
	if (unitRay.direction().y != unitRay.direction().y)	// fending against #INV camera direction
		return NULL;
    ContactManager* contactManager = NULL;
    if(workspace && workspace->getWorld())
    {
        contactManager = workspace->getWorld()->getContactManager();
    }
    
    if (!contactManager)
        return NULL;
    

	std::vector<const Primitive *> ignorePrims;
	if (Camera *cam = workspace->getCamera())
    {
		if (CameraSubject *sub = cam->getCameraSubject())
        {
			sub->getSelectionIgnorePrimitives(ignorePrims);
		}
	}

	PartInstance* bestPart = getMousePart(	unitRay,
											*contactManager,
											ignorePrims,
											filter,
											hitWorld);

	if (!bestPart)
    {
		hitWorld = unitRay.origin() + 10000.0f * unitRay.direction();
	}
	return bestPart;
}

///////////////////////////////////////////////////////////////////////////////

PartInstance* MouseCommand::getMousePart(
							const RbxRay& unitRay,
							const ContactManager& contactManager,
							const std::vector<const Primitive*> &ignore,
							const HitTestFilter* filter,
							Vector3& hitPoint,
							float maxSearchGrid)
{
	if (!unitRay.direction().isUnit())
		return NULL;
	RbxRay searchRay = RbxRay::fromOriginAndDirection(unitRay.origin(), unitRay.direction() * maxSearch());
	
	Primitive* bestPrimitive = contactManager.getHit(	searchRay,
														&ignore,
														filter,
														hitPoint);

	return bestPrimitive ? PartInstance::fromPrimitive(bestPrimitive) : NULL;
}


PartInstance* MouseCommand::getMousePart(
							const RbxRay& unitRay,
							const ContactManager& contactManager,
							const Primitive* ignore,
							const HitTestFilter* filter,
							Vector3& hitPoint,
							float maxSearchGrid)
{
	std::vector<const Primitive *> ignorePrims;
	ignorePrims.push_back(ignore);
	return getMousePart(unitRay, contactManager, ignorePrims, filter, hitPoint, maxSearchGrid);
}


} // namespace
