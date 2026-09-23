/* Copyright 2014 ROBLOX Corporation, All Rights Reserved */
#pragma once

#include "util/BinaryString.h"
#include "v8tree/Instance.h"
#include "v8tree/Service.h"
#include "v8datamodel/FlyweightService.h"

#include <boost/unordered_map.hpp>
#include "Value.h"

namespace ARL
{
	class PartOperation;

	extern const char *const sNonReplicatedCSGDictionaryService;

	class NonReplicatedCSGDictionaryService
		: public DescribedCreatable<NonReplicatedCSGDictionaryService, FlyweightService, sNonReplicatedCSGDictionaryService, Reflection::ClassDescriptor::PERSISTENT, Security::ANORRL>
	{
	protected:
		typedef DescribedCreatable<NonReplicatedCSGDictionaryService, FlyweightService, sNonReplicatedCSGDictionaryService, Reflection::ClassDescriptor::PERSISTENT, Security::ANORRL> Super;

		virtual void refreshRefCountUnderInstance(ARL::Instance* instance);

		void reparentChildData(shared_ptr<ARL::Instance> childInstance);

	public:

		NonReplicatedCSGDictionaryService();

		void storeData(PartOperation& partOperation, bool forceIncrement = false);
		void retrieveData(PartOperation& partOperation);

		void storeAllDescendants(shared_ptr<ARL::Instance> instance);
		void retrieveAllDescendants(shared_ptr<ARL::Instance> instance);
	};
}
