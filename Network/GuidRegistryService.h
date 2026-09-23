#pragma once

#include "v8tree/Service.h"

namespace ARL { 

	namespace Network {

		extern const char* const sGuidRegistryService;
		class GuidRegistryService 
			: public DescribedNonCreatable<GuidRegistryService, Instance, sGuidRegistryService>
			, public Service
		{
		public:
			boost::intrusive_ptr<GuidItem<Instance>::Registry> const registry;
			GuidRegistryService(void);
			~GuidRegistryService(void);
		};
	}
}
