#pragma once

#include "V8Tree/Service.h"

namespace ARL {

	extern const char* const sReplicatedStorage;

	class ReplicatedStorage
		: public DescribedCreatable<ReplicatedStorage, Instance, sReplicatedStorage, Reflection::ClassDescriptor::PERSISTENT_HIDDEN>
		, public Service
	{
	public:
		ReplicatedStorage();
	};

}
