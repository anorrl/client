#pragma once

#include "V8Tree/Service.h"
#include "script/IScriptFilter.h"

namespace ARL {

	extern const char* const sServerScriptService;

	class ServerScriptService 
		: public DescribedCreatable<ServerScriptService, Instance, sServerScriptService, Reflection::ClassDescriptor::PERSISTENT_LOCAL_INTERNAL>
		, public Service
		, public IScriptFilter
	{
	public:
		ServerScriptService();

		////////////////////////////////////////////////////////////////////////////////////
		// 
		// IScriptFilter
		/*override*/ bool scriptShouldRun(BaseScript* script);

	private:
		////////////////////////////////////////////////////////////////////////////////////
		// 
		// Instance
		/*override*/ bool askSetParent(const Instance* instance) const { return false; }
		/*override*/ bool askForbidParent(const Instance* instance) const { return !askSetParent(instance); }
		/*override*/ bool askAddChild(const Instance* instance) const;
		/*override*/ bool askForbidChild(const Instance* instance) const { return !askAddChild(instance); }
	};

}
