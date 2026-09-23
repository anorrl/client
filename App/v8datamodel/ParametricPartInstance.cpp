/* Copyright 2003-2005 ROBLOX Corporation, All Rights Reserved */
#include "stdafx.h"

#include "v8datamodel/ParametricPartInstance.h"
#include "v8world/Primitive.h"
#include "v8world/Poly.h"

namespace ARL
{
namespace PART
{

const char* const sWedge = "WedgePart";

ParametricPartInstance::ParametricPartInstance()
{
}

ParametricPartInstance::~ParametricPartInstance()
{
}

Wedge::Wedge()
{
	setName("Wedge");
	Primitive* myPrim = this->getPartPrimitive();
	myPrim->setGeometryType(Geometry::GEOMETRY_WEDGE);
	myPrim->setSurfaceType(NORM_Y, NO_SURFACE);
}

Wedge::~Wedge()
{
}


}} //namespace