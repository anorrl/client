/*
 *  RbxCrash.cpp
 *  MacClient
 *
 *  Created by elebel on 7/20/10.
 *  Copyright 2010 Roblox. All rights reserved.
 *
 */

#include "rbx/Debug.h"

void ARLCRASH()
{
	ARL::Debugable::doCrash();
}

void ARLCRASH(const char* message)
{
	ARL::Debugable::doCrash(message);
}
