#include "stdafx.h"

#include "v8world/ContactManagerSpatialHash.h"
#include "v8world/ContactManager.h"
#include "v8world/Contact.h"
#include "v8world/Assembly.h"


// instantiate for contactmanager.
template class ARL::SpatialHash<ARL::Primitive, ARL::Contact, ARL::ContactManager, CONTACTMANAGER_MAXLEVELS>;

#define CONTACT_MANAGER_MAX_CELLS_PER_PRIMITIVE 100

namespace ARL
{

	ContactManagerSpatialHash::ContactManagerSpatialHash(World* world, ContactManager* contactManager)
		: SpatialHash<Primitive,Contact,ContactManager, CONTACTMANAGER_MAXLEVELS>(world, contactManager, CONTACT_MANAGER_MAX_CELLS_PER_PRIMITIVE)	// max cells per primitive is 100 for contact manager
	{
	}

}
