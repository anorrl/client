#include "stdafx.h"

#include "ANORRLServicesTools.h"
#include "format_string.h"
#include <boost/algorithm/string/predicate.hpp>
#include <sstream>

#define DEFAULT_URL_SCHEMA "https"

std::string trim_trailing_slashes(const std::string &path) 
{
	size_t i = path.find_last_not_of('/');
	return path.substr(0, i+1);
}

static std::string BuildGenericApiUrl(const std::string &baseUrl, const std::string &path, const std::string &key, const char* scheme = "https")
{
    std::string rbxUrl = ".anorrl.com";
	size_t pos = baseUrl.find(rbxUrl);
	std::string subUrl = baseUrl.substr(0, pos);
    
	if (subUrl == "www" || subUrl == "http://www" || subUrl == "https://www" || subUrl == "m" || subUrl == "http://m" ) //prod
	{
		subUrl = "";
	}
	std::string httpPrefix = "https://";
	pos = subUrl.find(httpPrefix);
	if (pos != -1)
	{
		subUrl = subUrl.substr(httpPrefix.length(), subUrl.length() - httpPrefix.length());
	}

	std::string url;
	if (subUrl.empty())
	{
		// production
		url = format_string("%s://www.anorrl.com/%s/?apiKey=%s", scheme, path.c_str(), key.c_str());
	}
	else
	{
		if (subUrl.find("www") != -1)
		{
			subUrl = subUrl.replace(0, 4, "");
			url = format_string("%s://www.%s%s/%s/?apiKey=%s", scheme, subUrl.c_str(), rbxUrl.c_str(), path.c_str(), key.c_str());
		}
        else if(subUrl.find("m.") == 0)
        {
            subUrl = subUrl.replace(0, 2, "");
			url = format_string("%s://www.%s%s/%s/?apiKey=%s", scheme, subUrl.c_str(), rbxUrl.c_str(), path.c_str(), key.c_str());
        }
		else if (subUrl.find(".sitetest3") != -1) // Special case for URLs like alberto.sitetest3, navin.sitetest3, etc..
		{
			url = format_string("%s://www.sitetest3%s/%s/?apiKey=%s", scheme, rbxUrl.c_str(), path.c_str(), key.c_str());
		}
	}

	return url;
}

std::string GetCountersUrl(const std::string &baseUrl, const std::string &key)
{
	return BuildGenericApiUrl(baseUrl, "v1.1/Counters/Increment", key, DEFAULT_URL_SCHEMA);
}

std::string GetCountersMultiIncrementUrl(const std::string &baseUrl, const std::string &key)
{
	return BuildGenericApiUrl(baseUrl, "v1.0/MultiIncrement", key, DEFAULT_URL_SCHEMA);
}

std::string GetSettingsUrl(const std::string &baseUrl, const std::string &group, const std::string &key)
{
	return BuildGenericApiUrl(baseUrl, format_string("Setting/QuietGet/%s", group.c_str()), key, DEFAULT_URL_SCHEMA);
}

std::string GetSecurityKeyUrl(const std::string &baseUrl, const std::string &key)
{
	return BuildGenericApiUrl(baseUrl, "GetAllowedSecurityVersions", key);
}

std::string GetSecurityKeyUrl2(const std::string &baseUrl, const std::string &key)
{
	return BuildGenericApiUrl(baseUrl, "GetAllowedSecurityKeys", key);
}

// used by bootstrapper
std::string GetClientVersionUploadUrl(const std::string &baseUrl, const std::string &key)
{
	return BuildGenericApiUrl(baseUrl, "GetCurrentClientVersionUpload", key);
}

std::string GetPlayerGameDataUrl(const std::string &baseUrl, int userId, const std::string &key)
{
	return BuildGenericApiUrl(baseUrl, format_string("game/players/%d", userId), key);
}

std::string GetWebChatFilterURL(const std::string& baseUrl, const std::string& key)
{
	return BuildGenericApiUrl(baseUrl, "moderation/filtertext", key);
}

std::string GetMD5HashUrl(const std::string &baseUrl, const std::string &key)
{
	return BuildGenericApiUrl(baseUrl, "GetAllowedMD5Hashes", key);
}

std::string GetMemHashUrl(const std::string &baseUrl, const std::string &key)
{
	return BuildGenericApiUrl(baseUrl, "GetAllowedMemHashes", key);
}

std::string GetGridUrl(const std::string &anyUrl, bool changeToDataDomain)
{
    std::string url = trim_trailing_slashes(anyUrl);
    url = url + "/Error/Grid.ashx";
    return url;
}

std::string GetDmpUrl(const std::string &anyUrl, bool changeToDataDomain)
{
    std::string url = trim_trailing_slashes(anyUrl);
	url = url + "/Error/Dmp.ashx";
    return url;
}

std::string GetBreakpadUrl(const std::string &anyUrl, bool changeToDataDomain)
{
    std::string url = trim_trailing_slashes(anyUrl);
	url = url + "/Error/Breakpad.ashx";
    return url;
}

std::string BuildGenericPersistenceUrl(const std::string& baseUrl, const std::string &servicePath)
{
    std::string constructedURLDomain(baseUrl);
    std::string constructedServicePath(servicePath);

	if (!boost::algorithm::ends_with(constructedURLDomain, "/"))
    {
        constructedURLDomain.append("/");
    }
    return constructedURLDomain + constructedServicePath + "/" ;
}

std::string BuildGenericGameUrl(const std::string& baseUrl, const std::string &servicePath)
{
    std::string constructedURLDomain(baseUrl);
    std::string constructedServicePath(servicePath);
	
	if (!boost::algorithm::ends_with(constructedURLDomain, "/"))
    {
        constructedURLDomain.append("/");
    }
    if (boost::algorithm::starts_with(constructedServicePath, "/"))
    {
        constructedServicePath.erase(constructedServicePath.begin());
    }
    return constructedURLDomain + constructedServicePath;
}

