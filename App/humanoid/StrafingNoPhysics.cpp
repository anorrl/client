/* Copyright 2003-2005 ROBLOX Corporation, All Rights Reserved */
#include "stdafx.h"

#include "humanoid/StrafingNoPhysics.h"
#include "humanoid/Humanoid.h"

namespace ARL {
namespace HUMAN {

const char* const sStrafingNoPhysics = "StrafingNoPhysics";


StrafingNoPhysics::StrafingNoPhysics(Humanoid* humanoid, StateType priorState)
	:Named<MovingNoPhysicsBase, sStrafingNoPhysics>(humanoid, priorState)
{
}


} // namespace HUMAN
} // namespace ARL
