#include "stdafx.h"

#include "v8datamodel/RobloxReplicatedStorage.h"

using namespace ARL;

const char* const ARL::sRobloxReplicatedStorage = "RobloxReplicatedStorage";

RobloxReplicatedStorage::RobloxReplicatedStorage(void)
{
	setName(sRobloxReplicatedStorage);
	setRobloxLocked(true);
}
