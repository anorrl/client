/* Copyright 2003-2005 ROBLOX Corporation, All Rights Reserved */
#include "stdafx.h"

#include "v8world/IWorldStage.h"
#include "v8world/Contact.h"
#include "v8world/Edge.h"

namespace ARL {

void IWorldStage::onEdgeAdded(Edge* e) 
{
	ARLASSERT(getDownstreamWS());
	e->putInStage(this);
	getDownstreamWS()->onEdgeAdded(e);
}

void IWorldStage::onEdgeRemoving(Edge* e) 
{
	ARLASSERT(getDownstreamWS());
	getDownstreamWS()->onEdgeRemoving(e);
	e->removeFromStage(this);
}

} // namespace
