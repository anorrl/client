#include "stdafx.h"

#include "v8World/ContactManagerSpatialHash.h"
#include "v8World/ContactManager.h"
#include "v8World/Contact.h"
#include "v8World/Assembly.h"


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
