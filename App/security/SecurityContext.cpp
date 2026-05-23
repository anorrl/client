#include "stdafx.h"

#include "security/SecurityContext.h"
#include "arl/Thread.hpp"
#include "arl/Debug.h"

namespace ARL
{
	namespace Security
	{
		bool Context::isInRole(Identities identity, Permissions p)
		{
			if (p == None)
				return true;

			switch (identity)
			{
			case Anonymous:
			case GameScript_:
				return false;
#if defined(ARL_STUDIO_BUILD)
            case StudioPlugin:
                return p == Plugin;
#endif
			case GameScriptInANORRLPlace_:
				return                p == ANORRLPlace;
			case ANORRLGameScript_:
				return p == Plugin || p == ANORRLPlace || p == LocalUser ||                      p == ANORRLScript;
			case LocalGUI_:
			case CmdLine_:
				return p == Plugin || p == ANORRLPlace || p == LocalUser;
			case Replicator_:
				return                p == ANORRLPlace ||                   p == WritePlayer  || p == ANORRLScript;
			case COM:
			case WebService:
				return true;
			default:
				ARLASSERT(false);
				return false;
			}
		}

		Context& Context::current()
		{
			Context* t = ptr().get();
			if (!t)
			{
				static Context anonymous(Anonymous);
				t = &anonymous;
				ptr().reset(t);
			}
			return *t;
		}


		void Context::tssCleanup(Context*)
		{

		}

		boost::thread_specific_ptr<Context>& Context::ptr()
		{
			static boost::thread_specific_ptr<Context> value(tssCleanup);
			return value;
		}

		Impersonator::Impersonator(Identities identity)
			  : current(identity)
		{
			previous = Context::ptr().get();
			Context::ptr().reset(&current);
		}

		Impersonator::~Impersonator()
		{
			Context::ptr().reset(previous);
		}


	}
}
