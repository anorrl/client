// dllmain.h : Declaration of module class.

class CANORRLProxyModule : public CAtlDllModuleT< CANORRLProxyModule >
{
public :
	DECLARE_LIBID(LIBID_ANORRLProxyLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_ANORRLPROXY, "{664B192B-D17A-4921-ABF9-C6F6264E5110}")
};

extern class CANORRLProxyModule _AtlModule;
