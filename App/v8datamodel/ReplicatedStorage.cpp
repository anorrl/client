#include "stdafx.h"

#include "v8datamodel/ReplicatedStorage.h"

using namespace ARL;

const char* const ARL::sReplicatedStorage = "ReplicatedStorage";

ReplicatedStorage::ReplicatedStorage(void)
{
	setName(sReplicatedStorage);
}
