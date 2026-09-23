#include "SystemUtil.h"
#include "StreamHelpers.h"
#include "FastLog.h"
#include "ARLFormat.h"

#include <fstream>
#include <string>

#include <filesystem>
#include <stdio.h>
#include <stdlib.h>
#include <glad/gl.h>


#include <dirent.h>
#include <unistd.h>


using namespace ARL;

namespace ARL
{
namespace SystemUtil
{
std::string mOSVersion; // Set in JNIGLActivity.cpp
std::string mDeviceName;

std::string getCPUMake()
{
#ifdef __arm_
	return "ARM";
#elif __aarch64__
	return "AArch64";
#elif __i386__ || __amd64__
	return "Intel";
#else
#error Unsupported platform.
#endif
}

uint64_t getCPUSpeed()
{
	return 0;
}

uint64_t getCPULogicalCount()
{
	return getCPUCoreCount();
}

uint64_t getCPUCoreCount()
{
	return sysconf(_SC_NPROCESSORS_CONF);
}

uint64_t getCPUPhysicalCount()
{
	return getCPUCoreCount();
}

bool isCPU64Bit()
{
#ifdef __arm64__ || __amd64__
	return true;
#else
	return false;
#endif
}

uint64_t getMBSysRAM()
{
	return 16 * 1024;
}

uint64_t getMBSysAvailableRAM()
{
	return getMBSysRAM();
}

uint64_t getVideoMemory()
{
	GLint total_mem_kb = 0;
	// return 8gb
	return 8 * 1024 * 1024;
}

std::string osPlatform()
{
	return "Linux";
}

int osPlatformId()
{
	return 0x7999999999;
}

std::string osVer()
{
	return mOSVersion;
}

std::string deviceName()
{
	return ARL::SystemUtil::mDeviceName;
}

std::string getGPUMake()
{
	return "Im a Linux";
}

std::string getMaxRes()
{
	return "";
}

std::string getExecutablePath()
{
	char buffer[PATH_MAX] = {0};
	readlink("/proc/self/exe", buffer, sizeof(buffer));
	return std::filesystem::path(buffer).parent_path();
}
} // namespace SystemUtil
} // ARL