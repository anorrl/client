#include <stdio.h>
#include <comdef.h>

#include "ANORRLProxy_i.h"


const IID IID_ILauncher =
{0x699F0898,0xB7BC,0x4DE5,{0xAF,0xEE,0x0E,0xC3,0x8A,0xD4,0x22,0x40}};

#ifdef _WIN64
	const CLSID CLSID_Launcher =
	{0xDEE03C2B,0x0C0C,0x41A9,{0x98,0x77,0xFD,0x4B,0x4D,0x7B,0x6E,0xA3}};
#else
	const CLSID CLSID_Launcher =
	{0x76D50904,0x6780,0x4c8b,{0x89,0x86,0x1A,0x7E,0xE0,0xB1,0x71,0x6D}};
#endif

//=================================================================================
//=================COM Synchronization routing functions========================================
//=================================================================================

bool isComInitialized = false;

void StartCom(void){
	HRESULT	hr;
	if( !isComInitialized ){
		hr = CoInitialize(0);
		if( SUCCEEDED(hr) ){
			isComInitialized = true;
		}
	}
}

void StopCom(void){
	if( isComInitialized ){
		CoUninitialize();
		isComInitialized = false;
	}
}

bool isComStarted(void){
	return isComInitialized;
}

//=================================================================================
//=================String routing functions========================================
//=================================================================================

/**
* Conversion from ASCII to WideChar
*
*/

int Str2Wc( char* pBuf, WCHAR* pwcBuf, int bufLen){
	if( pwcBuf == NULL || pBuf == NULL )
		return -1;

	// If zero then assume the user has allocated enough
	if( bufLen == 0 ){
		bufLen = (int)( strlen(pBuf) + 1 );
	}
	if( MultiByteToWideChar( CP_ACP, 0, pBuf, -1, pwcBuf, bufLen) == 0 ){
		return -1;
	}
	pwcBuf[strlen(pBuf)] = 0;
	return 0;
}
/**
* Conversion from WideChar to ASCII
*
*/
int Wc2Str(WCHAR* pwcBuf, char* pBuf, int bufLen){
	if( pwcBuf == NULL || pBuf == NULL )
		return -1;

// If zero then assume the user has allocated enough
	if( bufLen == 0 ){
		bufLen = (int)( wcslen( pwcBuf) + 1 );
	}
	if( WideCharToMultiByte( CP_ACP, 0, pwcBuf, -1, pBuf, bufLen, NULL, NULL) == 0 ){
		return -1;
	}
	pBuf[wcslen(pwcBuf)] = 0;
	return 0;
}

//=================================================================================
//=================ANORRL ActiveX functions call===================================
//=================================================================================

/**
* Create ANORRL ActiveX instance
*
*/

ILauncher * GetIANORRLLauncherInstance(void){
	// Declare and HRESULT and a pointer to the ANORRLProxy interface
	HRESULT	hr;
	ILauncher * IANORRLLauncher;
	if( isComStarted() ){
		hr = CoCreateInstance( CLSID_Launcher, NULL, CLSCTX_INPROC_SERVER,
						IID_ILauncher, (void**) &IANORRLLauncher);

		if( !SUCCEEDED(hr) ){
			IANORRLLauncher=NULL;
		}
	}
	return IANORRLLauncher;

}

/**
* Invoke HelloWorld method from ANORRL ActiveX
*
*/
int ANORRLAdapterHelloWorld(ILauncher *iIANORRLLauncher){
	HRESULT	hr = iIANORRLLauncher->HelloWorld();
	return hr;
}

/**
* Invoke Hello method from ANORRL ActiveX
*
*/
int ANORRLAdapterHello(ILauncher *iIANORRLLauncher,char *retMessage){
	HRESULT			hr;
	BSTR hhh;

	hr = -1;
	hr = iIANORRLLauncher->Hello( &hhh );
	if( SUCCEEDED(hr) ){
		Wc2Str( hhh, retMessage, 0 );
	}
	else {
		strcpy( retMessage, "IANORRLLauncher->Hello() Error occured !" );
	}

	return hr;
}
/**
* Invoke StartGame method from ANORRL ActiveX
*
*/
int ANORRLAdapterStartGame(ILauncher *iIANORRLLauncher,char *cbufAuth, char *cbufScript ){
	HRESULT			hr;
	_bstr_t authenticationUrl(cbufAuth);
	_bstr_t script(cbufScript);

	hr = -1;
	hr = iIANORRLLauncher->StartGame( authenticationUrl.GetBSTR(), 
									script.GetBSTR() );

	return hr;
}

/**
* Invoke PreStartGame method from ANORRL ActiveX
*
*/
int ANORRLAdapterPreStartGame(ILauncher *iIANORRLLauncher){
	HRESULT hr = -1;

	hr = iIANORRLLauncher->PreStartGame();

	return hr;
}

/**
* Invoke GetInstallHost method from ANORRL ActiveX
*
*/
int ANORRLAdapterGet_InstallHost(ILauncher *iIANORRLLauncher,char *cbufHost){
	HRESULT			hr;
	BSTR host;
	hr = -1;
	hr = iIANORRLLauncher->get_InstallHost( &host );

	if( SUCCEEDED(hr) ){
		Wc2Str( host, cbufHost, 0 );
	}
	else {
		strcpy( cbufHost, "IANORRLLauncher->get_InstallHost() Error occured !" );
	}


	return hr;
}

/**
* Invoke GetVersion method from ANORRL ActiveX
*
*/
int ANORRLAdapterGet_Version(ILauncher *iIANORRLLauncher,char *cbufHost) {
	HRESULT			hr;
	BSTR host;
	hr = -1;
	hr = iIANORRLLauncher->get_Version( &host );

	if( SUCCEEDED(hr) ){
		Wc2Str( host, cbufHost, 0 );
	}
	else {
		strcpy( cbufHost, "IANORRLLauncher->get_Version() Error occured !" );
	}


	return hr;
}

/**
* Invoke Update method from ANORRL ActiveX
*
*/
int ANORRLAdapterUpdate(ILauncher *iIANORRLLauncher){
	HRESULT			hr;
	hr = iIANORRLLauncher->Update();

	return hr;
}

/**
* Invoke PutAuthentificationTicket method from ANORRL ActiveX
*
*/
int ANORRLAdapterPut_AuthenticationTicket(ILauncher *iIANORRLLauncher,char *cbufTicket ){
	HRESULT			hr;
	_bstr_t ticket(cbufTicket);

	hr = -1;
	hr = iIANORRLLauncher->put_AuthenticationTicket(ticket.GetBSTR());

	return hr;
}

/**
* Invoke IsGameStarted method from ANORRL ActiveX
*
*/
int ANORRLAdapterGet_IsGameStarted(ILauncher *iIANORRLLauncher, bool *isStarted){
	HRESULT			hr;
	VARIANT_BOOL started;

	hr = -1;
	hr = iIANORRLLauncher->get_IsGameStarted(&started);
	(*isStarted) = (started == VARIANT_TRUE);

	return hr;
}

/**
* Invoke SetSilentModeEnabled method from ANORRL ActiveX
*
*/
int ANORRLAdapterGet_SetSilentModeEnabled(ILauncher *iIANORRLLauncher, bool silendMode){
	HRESULT			hr;
	VARIANT_BOOL silent = silendMode ? VARIANT_TRUE : VARIANT_FALSE;

	hr = -1;
	hr = iIANORRLLauncher->SetSilentModeEnabled(silent);

	return hr;
}

/**
* Invoke UnhideApp method from ANORRL ActiveX
*
*/
int ANORRLAdapterGet_UnhideApp(ILauncher *iIANORRLLauncher)
{
	HRESULT			hr;

	hr = -1;
	hr = iIANORRLLauncher->UnhideApp();

	return hr;
}

/**
* Invoke BringAppToFront method from ANORRL ActiveX
*
*/
int ANORRLAdapterBringAppToFront(ILauncher *iIANORRLLauncher)
{
	HRESULT			hr;

	hr = -1;
	hr = iIANORRLLauncher->BringAppToFront();

	return hr;
}

/**
* Invoke ResetLaunchState method from ANORRL ActiveX
*
*/
int ANORRLAdapterResetLaunchState(ILauncher *iIANORRLLauncher)
{
	HRESULT			hr;

	hr = -1;
	hr = iIANORRLLauncher->ResetLaunchState();

	return hr;
}

/**
* Invoke SetEditMode method from ANORRL ActiveX
*
*/
int ANORRLAdapterSetEditMode(ILauncher *iIANORRLLauncher)
{
	HRESULT			hr;

	hr = -1;
	hr = iIANORRLLauncher->SetEditMode();

	return hr;
}

/**
* Invoke SetStartInHiddenMode method from ANORRL ActiveX
*
*/
int ANORRLAdapterGet_SetStartInHiddenMode(ILauncher *iIANORRLLauncher, bool hiddenMode){
	HRESULT			hr;
	VARIANT_BOOL hidden = hiddenMode ? VARIANT_TRUE : VARIANT_FALSE;

	hr = -1;
	hr = iIANORRLLauncher->SetStartInHiddenMode(hidden);

	return hr;
}

/**
* Invoke get_IsUpToDate method from ANORRL ActiveX
*
*/
int ANORRLAdapterGet_IsUpToDate(ILauncher *iIANORRLLauncher, bool *isUpToDate){
	int hr;
	VARIANT_BOOL upToDate = VARIANT_FALSE;

	hr = -1;
	hr = iIANORRLLauncher->get_IsUpToDate(&upToDate);

	*isUpToDate = (upToDate == VARIANT_TRUE);
	
	return hr;
}

/**
* Invoke GetKeyValue method from ANORRL ActiveX
*
*/
int ANORRLAdapterGetKeyValue(ILauncher *iIANORRLLauncher, char *key, char *value)
{
	int hr = -1;
	_bstr_t bstrKey(key);
	BSTR bstrValue;
	VARIANT_BOOL exists = VARIANT_FALSE;

	hr = iIANORRLLauncher->GetKeyValue(bstrKey, &bstrValue);
	if (SUCCEEDED(hr))
	{
		Wc2Str(bstrValue, value, 0);
	} else {
		value = NULL;
	}

	return hr;
}

/**
* Invoke SetKeyValue method from ANORRL ActiveX
*
*/
int ANORRLAdapterSetKeyValue(ILauncher *iIANORRLLauncher, char *key, char *value)
{
	_bstr_t bstrKey(key);
	_bstr_t bstrValue(value);

	return iIANORRLLauncher->SetKeyValue(bstrKey, bstrValue);
}

/**
* Invoke DeleteKeyValue method from ANORRL ActiveX
*
*/
int ANORRLAdapterDeleteKeyValue(ILauncher *iIANORRLLauncher, char *key)
{
	_bstr_t bstrKey(key);

	return iIANORRLLauncher->DeleteKeyValue(bstrKey);
}