
#ifdef ARLTESTHOOKS_EXPORTS
#define ARLTESTHOOKS_API __declspec(dllexport)
#else
#define ARLTESTHOOKS_API __declspec(dllimport)
#endif

namespace ARL
{

	class IWorkspaceTestHooks
	{
	public:
		virtual HRESULT ExecScriptImpl(BSTR script, VARIANT arg1, VARIANT arg2, VARIANT arg3, VARIANT arg4, SAFEARRAY** result) = 0;
	};

}

ARLTESTHOOKS_API HRESULT IWorkspaceTestHooks_ExecScript(ARL::IWorkspaceTestHooks *p, BSTR script, VARIANT arg1, VARIANT arg2, VARIANT arg3, VARIANT arg4, SAFEARRAY** result);
