#pragma once

#include <string>
#include <comdef.h>
#include "format_string.h"
#include <boost/noncopyable.hpp>
#include <atlsync.h>

class CTimedMutexLock : public boost::noncopyable
{
public:
	CTimedMutexLock( CMutex& mtx ) :
	  m_mtx( mtx ),
		  m_bLocked( false )
	  {
	  }

	  ~CTimedMutexLock() throw()
	  {
		  if( m_bLocked )
		  {
			  Unlock();
		  }
	  }

	  DWORD Lock(DWORD timeout)
	  {
		  DWORD dwResult;

		  ATLASSERT( !m_bLocked );
		  dwResult = ::WaitForSingleObject( m_mtx, timeout );
		  if( dwResult == WAIT_ABANDONED )
		  {
			  ATLTRACE(atlTraceSync, 0, _T("Warning: abandoned mutex 0x%x\n"), (HANDLE)m_mtx);
		  }
		  if ( dwResult != WAIT_TIMEOUT )
		  {
			  m_bLocked = true;
		  }
		  return dwResult;
	  }

	  void Unlock() throw()
	  {
		  ATLASSUME( m_bLocked );

		  m_mtx.Release();
		  //ATLASSERT in CMutexLock::Lock prevents calling Lock more than 1 time.
		  m_bLocked = false;  
	  }

	  // Implementation
private:
	CMutex& m_mtx;
	bool m_bLocked;
};

#define LOG_ENTRY(msg) logger.write_logentry(msg)
#define LOG_ENTRY1(msg, a1) logger.write_logentry(msg, reinterpret_cast<const void*>(a1))
#define LOG_ENTRY2(msg, a1, a2) logger.write_logentry(msg, reinterpret_cast<const void*>(a1), reinterpret_cast<const void*>(a2))
#define LOG_ENTRY3(msg, a1, a2, a3) logger.write_logentry(msg, reinterpret_cast<const void*>(a1), reinterpret_cast<const void*>(a2), reinterpret_cast<const void*>(a3))
#define LOG_ENTRY4(msg, a1, a2, a3, a4) logger.write_logentry(msg, reinterpret_cast<const void*>(a1), reinterpret_cast<const void*>(a2), reinterpret_cast<const void*>(a3), reinterpret_cast<const void*>(a4))

#define LLOG_ENTRY(logger, msg) logger.write_logentry(msg)
#define LLOG_ENTRY1(logger, msg, a1) logger.write_logentry(msg, reinterpret_cast<const void*>(a1))
#define LLOG_ENTRY2(logger, msg, a1, a2) logger.write_logentry(msg, reinterpret_cast<const void*>(a1), reinterpret_cast<const void*>(a2))
#define LLOG_ENTRY3(logger, msg, a1, a2, a3) logger.write_logentry(msg, reinterpret_cast<const void*>(a1), reinterpret_cast<const void*>(a2), reinterpret_cast<const void*>(a3))
#define LLOG_ENTRY4(logger, msg, a1, a2, a3, a4) logger.write_logentry(msg, reinterpret_cast<const void*>(a1), reinterpret_cast<const void*>(a2), reinterpret_cast<const void*>(a3), reinterpret_cast<const void*>(a4))

#define FIREFOXREGKEY               "@nslambda.lambda.cam/launcher"
#define FIREFOXREGKEY64             "@nslambda.lambda.cam/launcher64"

#define PLAYER2010LINKNAME_CUR      "ANORRL App 2010"
#define PLAYER2013LINKNAME_CUR      "ANORRL Player 2013"
#define PLAYERLINKNAME_CUR          "ANORRL Player"
#define PLAYERLINKNAMELEGACY        "Open ANORRL Games"

// MFC Studio names
#define STUDIOEXENAME               "RobloxStudio.exe"
#define STUDIOBOOTSTAPPERNAME       "RobloxStudioLauncher.exe"
#define STUDIOLINKNAMELEGACY        "Roblox Studio"    // wrong case

// QT Studio names
#define STUDIOQTEXENAME             "ANORRLStudioBeta.exe"
#define STUDIOBOOTSTAPPERNAMEBETA   "ANORRLStudioLauncherBeta.exe"
#define STUDIO2013QTLINKNAME_CUR    "ANORRL Studio 2013"
#define STUDIOQTLINKNAME_CUR        "ANORRL Studio"
#define STUDIOQTLINKNAME            "ROBLOX Studio Beta"
#define STUDIOQTLINKNAME20          "ROBLOX Studio 2.0"
#define STUDIOQTLINKNAME20BETA      "ROBLOX Studio 2.0 Beta"
#define STUDIOQTLINKNAME2013        "ROBLOX Studio 2013"

// Player names
#define  PLAYEREXENAME				"ANORRLPlayerBeta.exe"
#define  PLAYER2010EXENAME			"ANORRLApp.exe"

// Version URL string names (setup.roblox.com/XXX) - replace XXX with these values to get the most recent version
// Unfortunately these have to be defined globally so Player knows how to find the latest version of Studio to download
#define VERSIONGUIDNAMESTUDIO       "versionQTStudio"
#define VERSIONGUIDNAME2013STUDIO   "versionQT2013Studio"
#define VERSIONGUIDNAMERCC			"NOVERSION"
#define VERSIONGUIDNAMEPLAYER		"version"
#define VERSIONGUIDNAME2013PLAYER	"version2013"
#define VERSIONGUIDNAME2010PLAYER	"version2010"

CRegKey CreateKey(HKEY parent, const TCHAR* name, const TCHAR* defaultValue = NULL, bool is64bits = false);
void DeleteKey(simple_logger<wchar_t> &logger, HKEY parent, const TCHAR* name);
void DeleteSubKey(simple_logger<wchar_t> &logger, HKEY parent, const TCHAR* child, const TCHAR* key);
std::string QueryStringValue(CRegKey& key, const TCHAR* name);

void throwHRESULT(HRESULT hr, const char* message);
void throwHRESULT(HRESULT hr, const std::string& message);
void throwLastError(BOOL result, const std::string& message);
CString GetLastErrorDescription();

std::string exceptionToString(const _com_error& e, int stage);
std::string exceptionToString(const std::exception& e);

void copyFile(simple_logger<wchar_t> &logger, const TCHAR* srcFile, const TCHAR* dstFile);

void setCurrentVersion(simple_logger<wchar_t> &logger, bool isPerUser, const TCHAR* componentCode, const TCHAR* version, const TCHAR* baseUrl);
void getCurrentVersion(simple_logger<wchar_t> &logger, bool isPerUser, const TCHAR* componentCode, TCHAR* version, int vBufSize, TCHAR* baseUrl, int uBaseSize);
void deleteCurVersionKeys(simple_logger<wchar_t> &logger, bool isPerUser, const TCHAR *component);
std::wstring getPlayerCode();
std::wstring getStudioCode();
std::wstring getQTStudioCode();
std::wstring getPlayerInstallKey();
std::wstring getStudioInstallKey();
std::wstring getQTStudioInstallKey();

std::wstring get2013PlayerCode();
std::wstring getQT2013StudioCode();
std::wstring get2013PlayerInstallKey();
std::wstring getQT2013StudioInstallKey();

std::wstring get2010PlayerCode();
std::wstring get2010PlayerInstallKey();

std::wstring getPlayerProtocolScheme(const std::string& baseUrl);
std::wstring getQTStudioProtocolScheme(const std::string& baseUrl);
std::wstring get2010PlayerProtocolScheme(const std::string& baseUrl);
std::wstring get2013PlayerProtocolScheme(const std::string& baseUrl);
std::wstring getQT2013StudioProtocolScheme(const std::string& baseUrl);
	
std::wstring getStudioRegistrySubPath();
std::wstring getStudioRegistryPath();
std::wstring getQTStudioRegistrySubPath();
std::wstring getQTStudioRegistryPath();

std::wstring getQT2013StudioRegistrySubPath();
std::wstring getQT2013StudioRegistryPath();

void createRobloxShortcut(simple_logger<wchar_t> &logger, bool isPerUser, const TCHAR *linkFileName, const TCHAR *exePath, const TCHAR *args, bool desktop, bool forceCreate);
void updateExistingRobloxShortcuts(simple_logger<wchar_t> &logger, bool isPerUser, const TCHAR *folder, const TCHAR *exeName, const TCHAR *exeFolderPath, const TCHAR *baseFolderPath);
std::wstring getRobloxProgramsFolder(simple_logger<wchar_t> &logger, bool isPerUser);
std::wstring getDektopShortcutPath(simple_logger<wchar_t> &logger, const TCHAR *shortcutName);
bool hasDesktopShortcut(simple_logger<wchar_t> &logger, const TCHAR *shortcutName);
bool hasProgramShortcut(simple_logger<wchar_t> &logger,  bool isPerUser, const TCHAR *shortcutName);
bool deleteDesktopShortcut(simple_logger<wchar_t> &logger, const TCHAR *shortcutName);
void deleteProgramsShortcut(simple_logger<wchar_t> &logger,  bool isPerUser, const TCHAR *shortcutName);