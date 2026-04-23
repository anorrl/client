#ifndef _ROLBOX_ADAPTER_H_
#define _ROLBOX_ADAPTER_H_

void StartCom(void);
void StopCom(void);
bool isComStarted(void);

ILauncher * GetIANORRLLauncherInstance(void);

int ANORRLAdapterHelloWorld(ILauncher *iIANORRLLauncher);
int ANORRLAdapterHello(ILauncher *iIANORRLLauncher,char *retMessage);
int ANORRLAdapterStartGame(ILauncher *iIANORRLLauncher,char *cbufAuth, char *cbufScript );
int ANORRLAdapterPreStartGame(ILauncher *iIANORRLLauncher);
int ANORRLAdapterGet_InstallHost(ILauncher *iIANORRLLauncher,char *cbufHost);
int ANORRLAdapterGet_Version(ILauncher *iIANORRLLauncher,char *cbufHost);
int ANORRLAdapterUpdate(ILauncher *iIANORRLLauncher);
int ANORRLAdapterPut_AuthenticationTicket(ILauncher *iIANORRLLauncher,char *cbufTicket );
int ANORRLAdapterGet_IsGameStarted(ILauncher *iIANORRLLauncher, bool *isStarted);
int ANORRLAdapterGet_SetSilentModeEnabled(ILauncher *iIANORRLLauncher, bool silendMode);
int ANORRLAdapterGet_UnhideApp(ILauncher *iIANORRLLauncher);
int ANORRLAdapterBringAppToFront(ILauncher *iIANORRLLauncher);
int ANORRLAdapterResetLaunchState(ILauncher *iIANORRLLauncher);
int ANORRLAdapterSetEditMode(ILauncher *iIANORRLLauncher);
int ANORRLAdapterGet_SetStartInHiddenMode(ILauncher *iIANORRLLauncher, bool hiddenMode);
int ANORRLAdapterGet_IsUpToDate(ILauncher *iIANORRLLauncher, bool *isUpToDate);
int ANORRLAdapterGetKeyValue(ILauncher *iIANORRLLauncher, char *key, char *value);
int ANORRLAdapterSetKeyValue(ILauncher *iIANORRLLauncher, char *key, char *value);
int ANORRLAdapterDeleteKeyValue(ILauncher *iIANORRLLauncher, char *key);

#endif

