#pragma once
// licensing
enum VMProtectSerialStateFlags
{
	SERIAL_STATE_SUCCESS				= 0,
	SERIAL_STATE_FLAG_CORRUPTED			= 0x00000001,
	SERIAL_STATE_FLAG_INVALID			= 0x00000002,
	SERIAL_STATE_FLAG_BLACKLISTED		= 0x00000004,
	SERIAL_STATE_FLAG_DATE_EXPIRED		= 0x00000008,
	SERIAL_STATE_FLAG_RUNNING_TIME_OVER	= 0x00000010,
	SERIAL_STATE_FLAG_BAD_HWID			= 0x00000020,
	SERIAL_STATE_FLAG_MAX_BUILD_EXPIRED	= 0x00000040,
};

typedef struct
{
	unsigned short	wYear;
	unsigned char	bMonth;
	unsigned char	bDay;
} VMProtectDate;

typedef struct
{
	int				nState;				// VMProtectSerialStateFlags
	unsigned short		wUserName[256];		// user name
	unsigned short		wEMail[256];		// email
	VMProtectDate	dtExpire;			// date of serial number expiration
	VMProtectDate	dtMaxBuild;			// max date of build, that will accept this key
	int				bRunningTime;		// running time in minutes
	unsigned char	nUserDataLength;	// length of user data in bUserData
	unsigned char	bUserData[255];		// up to 255 bytes of user data
} VMProtectSerialNumberData;

// activation
enum VMProtectActivationFlags
{
	ACTIVATION_OK = 0,
	ACTIVATION_SMALL_BUFFER,
	ACTIVATION_NO_CONNECTION,
	ACTIVATION_BAD_REPLY,
	ACTIVATION_BANNED,
	ACTIVATION_CORRUPTED,
	ACTIVATION_BAD_CODE,
	ACTIVATION_ALREADY_USED,
	ACTIVATION_SERIAL_UNKNOWN,
	ACTIVATION_EXPIRED,
	ACTIVATION_NOT_AVAILABLE
};

#include <cstring>

inline void VMProtectBegin(const char *) {}
inline void VMProtectBeginVirtualization(const char *) {}
inline void VMProtectBeginMutation(const char *) {}
inline void VMProtectBeginUltra(const char *) {}
inline void VMProtectBeginVirtualizationLockByKey(const char *) {}
inline void VMProtectBeginUltraLockByKey(const char *) {}
inline void VMProtectEnd(void) {}

inline bool VMProtectIsProtected() { return false; }
inline bool VMProtectIsDebuggerPresent(bool) { return false; }
inline bool VMProtectIsVirtualMachinePresent(void) { return false; }
inline bool VMProtectIsValidImageCRC(void) { return true; }

inline const char *VMProtectDecryptStringA(const char *value) { return value; }
inline const unsigned short *VMProtectDecryptStringW(const unsigned short *value) { return value; }
inline bool VMProtectFreeString(const void *) { return true; }

inline int VMProtectSetSerialNumber(const char *) { return SERIAL_STATE_SUCCESS; }
inline int VMProtectGetSerialNumberState() { return SERIAL_STATE_SUCCESS; }

inline bool VMProtectGetSerialNumberData(VMProtectSerialNumberData *data, int size) {
	if (!data || size < (int)sizeof(VMProtectSerialNumberData)) return false;
	std::memset(data, 0, sizeof(VMProtectSerialNumberData));
	data->nState = SERIAL_STATE_SUCCESS;
	return true;
}

inline int VMProtectGetCurrentHWID(char *hwid, int size) {
	if (!hwid || size < 1) return ACTIVATION_SMALL_BUFFER;
	std::strncpy(hwid, "DUMMY_HWID", size);
	return ACTIVATION_OK;
}

inline int VMProtectActivateLicense(const char *code, char *serial, int size) {
	if (serial && size > 0) std::strncpy(serial, "DUMMY-SERIAL", size);
	return ACTIVATION_OK;
}

inline int VMProtectDeactivateLicense(const char *serial) {
	return ACTIVATION_OK;
}

inline int VMProtectGetOfflineActivationString(const char *code, char *buf, int size) {
	if (buf && size > 0) std::strncpy(buf, "OFFLINE-ACTIVATION", size);
	return ACTIVATION_OK;
}

inline int VMProtectGetOfflineDeactivationString(const char *serial, char *buf, int size) {
	if (buf && size > 0) std::strncpy(buf, "OFFLINE-DEACTIVATION", size);
	return ACTIVATION_OK;
}