 /* Copyright 2003-2005 ROBLOX Corporation, All Rights Reserved */

#pragma once

#include "v8world/SpatialHashMultiRes.h"

namespace ARL
{
	class Primitive;
	class Contact;
	class ContactManager;
	class World;
	class Assembly;
	
#define CONTACTMANAGER_MAXLEVELS 4
	class ContactManagerSpatialHash : public SpatialHash<Primitive, Contact, ContactManager, CONTACTMANAGER_MAXLEVELS>
	{
	public:
		ContactManagerSpatialHash(World* world, ContactManager* contactManager);
	};

}


