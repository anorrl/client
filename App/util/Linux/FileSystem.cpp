#include "util/FileSystem.h"
#include <boost/filesystem.hpp>
#include <boost/system/error_code.hpp>
#include <cstdio>

namespace ARL {
namespace FileSystem {

#ifndef ANDROID
boost::filesystem::path getUserDirectory(bool create, FileSystemDir dir, const char* subDirectory)
{
	boost::filesystem::path basePath;


	switch(dir)
	{
		case DirExe:
			basePath = boost::filesystem::current_path();
			break;
		case DirAppData:
		{
			const char* home = getenv("HOME");
			if (home)
				basePath = boost::filesystem::path(home) / ".local/share/ANORRL";
			else
				basePath = "/tmp/ANORRL"; // fallback
			break;
		}
		case DirPicture:
		{
			const char* home = getenv("HOME");
			if (home)
				basePath = boost::filesystem::path(home) / "Pictures/ANORRL";
			else
				basePath = "/tmp/ANORRL";
			break;
		}
		default:
		{
			const char* home = getenv("HOME");
			if (home)
				basePath = boost::filesystem::path(home) / "ANORRL";
			else
				basePath = "/tmp/ANORRL";
			break;
		}
	}

	if (subDirectory)
	{
		basePath /= subDirectory;
	}

	if (create)
	{
		boost::system::error_code ec;
		boost::filesystem::create_directories(basePath, ec);
		if (ec)
		{
			fprintf(stderr, "Failed to create directories: %s\n", ec.message().c_str());
		}
	}

	return basePath;
}
#endif

boost::filesystem::path getCacheDirectory(bool create, const char* subDirectory)
{
	return getUserDirectory(create, DirAppData, subDirectory);
}

boost::filesystem::path getTempFilePath()
{
	return getUserDirectory(true, DirAppData, "temp");
}

boost::filesystem::path getLogsDirectory()
{
	return getUserDirectory(true, DirAppData, "logs");
}

void clearCacheDirectory(const char* subDirectory)
{
	boost::filesystem::path cachePath = getCacheDirectory(false, subDirectory);
	namespace fs = boost::filesystem;
	boost::system::error_code ec;
	if (!cachePath.empty() && fs::exists(cachePath, ec) && !ec)
	{
		fs::directory_iterator end_iter;
		for (fs::directory_iterator iter(cachePath); end_iter != iter; ++iter)
		{
			if (!fs::is_directory(iter->status()))
			{
				boost::filesystem::remove(*iter, ec);
			}
		}
	}
}

} // namespace FileSystem
} // namespace ARL
