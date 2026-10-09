#include "stdafx.h"

#include "v8datamodel/ServerStorage.h"
#include "v8datamodel/GameBasicSettings.h"
#include "network/Players.h"

using namespace ARL;

const char* const ARL::sServerStorage = "ServerStorage";

ServerStorage::ServerStorage(void)
{
	setName(sServerStorage);
}

bool ServerStorage::askAddChild(const Instance* instance) const
{
	return ARL::Network::Players::backendProcessing(this) || ARL::GameBasicSettings::singleton().inStudioMode();
}