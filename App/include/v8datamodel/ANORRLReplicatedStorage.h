#pragma once

#include "v8tree/Service.h"

namespace ARL {

	extern const char* const sANORRLReplicatedStorage;

	class ANORRLReplicatedStorage
		: public DescribedCreatable<ANORRLReplicatedStorage, Instance, sANORRLReplicatedStorage, Reflection::ClassDescriptor::INTERNAL, Security::ANORRLScript>
		, public Service
	{
	public:
		ANORRLReplicatedStorage();
	};

}
