#include "stdafx.h"

#include "RobloxServicesTools.h"
#include "format_string.h"
#include <boost/algorithm/string/predicate.hpp>
#include <sstream>


#ifdef ARL_PLATFORM_DURANGO
#define DEFAULT_URL_SCHEMA "https"
#else
#define DEFAULT_URL_SCHEMA "http"
#endif

std::string trim_trailing_slashes(const std::string &path) 
{
	size_t i = path.find_last_not_of('/');
	return path.substr(0, i+1);
}

static std::string BuildGenericApiUrl(const std::string &baseUrl, const std::string &serviceNameIn, const std::string &path, const std::string &key, const char* scheme = "https")
{
    std::string serviceName(serviceNameIn);
    std::string rbxUrl = ".lambda.cam";
	size_t pos = baseUrl.find(rbxUrl);
	if (pos == std::string::npos)
	{
		rbxUrl = ".robloxlabs.com";
		pos =  baseUrl.find(rbxUrl);
	}

	std::string subUrl = baseUrl.substr(0, pos);
    
	if (subUrl == "arl" || subUrl == "http://arl" || subUrl == "https://arl" || subUrl == "m" || subUrl == "http://m" ) //prod
	{
		subUrl = "";
	}
	std::string httpPrefix = "http://";
	pos = subUrl.find(httpPrefix);
	if (pos != -1)
	{
		subUrl = subUrl.substr(httpPrefix.length(), subUrl.length() - httpPrefix.length());
	}
	
	if ("" != serviceName)
		serviceName += ".";

	std::string url;
	if (subUrl.empty())
	{
		// production
		url = format_string("%s://%sarl.lambda.cam/%s/?apiKey=%s", scheme, serviceName.c_str(), path.c_str(), key.c_str());
	}
	else
	{
		if (subUrl.find("arl") != -1)
		{
			subUrl = subUrl.replace(0, 4, "");
			url = format_string("%s://%sarl.%s%s/%s/?apiKey=%s", scheme, serviceName.c_str(), subUrl.c_str(), rbxUrl.c_str(), path.c_str(), key.c_str());
		}
        else if(subUrl.find("m.") == 0)
        {
            subUrl = subUrl.replace(0, 2, "");
			url = format_string("%s://%sarl.%s%s/%s/?apiKey=%s", scheme, serviceName.c_str(), subUrl.c_str(), rbxUrl.c_str(), path.c_str(), key.c_str());
        }
		else if (subUrl.find(".sitetest3") != -1) // Special case for URLs like alberto.sitetest3, navin.sitetest3, etc..
		{
			url = format_string("%s://%sarl.sitetest3%s/%s/?apiKey=%s", scheme, serviceName.c_str(), rbxUrl.c_str(), path.c_str(), key.c_str());
		}
	}

	return url;
}

std::string GetCountersUrl(const std::string &baseUrl, const std::string &key)
{
	//return BuildGenericApiUrl(baseUrl, "ephemeralcounters", "v1.1/Counters/Increment", key, DEFAULT_URL_SCHEMA);
	return BuildGenericApiUrl(baseUrl, "", "v1.1/Counters/Increment", key, DEFAULT_URL_SCHEMA);
}

std::string GetCountersMultiIncrementUrl(const std::string &baseUrl, const std::string &key)
{
	//return BuildGenericApiUrl(baseUrl, "ephemeralcounters", "v1.0/MultiIncrement", key, DEFAULT_URL_SCHEMA);
	return BuildGenericApiUrl(baseUrl, "", "v1.0/MultiIncrement", key, DEFAULT_URL_SCHEMA);
}

std::string GetSettingsUrl(const std::string &baseUrl, const std::string &group, const std::string &key)
{
	//return BuildGenericApiUrl(baseUrl, "clientsettings", format_string("Setting/QuietGet/%s", group.c_str()), key, DEFAULT_URL_SCHEMA);
	return BuildGenericApiUrl(baseUrl, "", format_string("Setting/QuietGet/%s", group.c_str()), key, DEFAULT_URL_SCHEMA);
}

std::string GetSecurityKeyUrl(const std::string &baseUrl, const std::string &key)
{
	//return BuildGenericApiUrl(baseUrl, "versioncompatibility", "GetAllowedSecurityVersions", key);
	return BuildGenericApiUrl(baseUrl, "", "GetAllowedSecurityVersions", key);
}

std::string GetSecurityKeyUrl2(const std::string &baseUrl, const std::string &key)
{
	//return BuildGenericApiUrl(baseUrl, "versioncompatibility", "GetAllowedSecurityKeys", key);
	return BuildGenericApiUrl(baseUrl, "", "GetAllowedSecurityKeys", key);
}

// used by bootstrapper
std::string GetClientVersionUploadUrl(const std::string &baseUrl, const std::string &key)
{
	//return BuildGenericApiUrl(baseUrl, "versioncompatibility", "GetCurrentClientVersionUpload", key);
	return BuildGenericApiUrl(baseUrl, "", "GetCurrentClientVersionUpload", key);
}

std::string GetPlayerGameDataUrl(const std::string &baseurl, int userId, const std::string &key)
{
	return BuildGenericApiUrl(baseurl, "", format_string("/game/players/%d", userId), key);
}

std::string GetWebChatFilterURL(const std::string& baseUrl, const std::string& key)
{
	return BuildGenericApiUrl(baseUrl, "", "/moderation/filtertext", key);
}

std::string GetMD5HashUrl(const std::string &baseUrl, const std::string &key)
{
	//return BuildGenericApiUrl(baseUrl, "versioncompatibility", "GetAllowedMD5Hashes", key);
	return BuildGenericApiUrl(baseUrl, "", "GetAllowedMD5Hashes", key);
}

std::string GetMemHashUrl(const std::string &baseUrl, const std::string &key)
{
	//return BuildGenericApiUrl(baseUrl, "versioncompatibility", "GetAllowedMemHashes", key);
	return BuildGenericApiUrl(baseUrl, "", "GetAllowedMemHashes", key);
}

std::string GetGridUrl(const std::string &anyUrl, bool changeToDataDomain)
{
    std::string url = trim_trailing_slashes(anyUrl);
	if (changeToDataDomain)
		url = ReplaceTopSubdomain(url, "arl"); //url = ReplaceTopSubdomain(url, "data");

    url = url + "/Error/Grid.ashx";
    return url;
}

std::string GetDmpUrl(const std::string &anyUrl, bool changeToDataDomain)
{
    std::string url = trim_trailing_slashes(anyUrl);
	if (changeToDataDomain)
		url = ReplaceTopSubdomain(url, "arl"); //url = ReplaceTopSubdomain(url, "data");
    
	url = url + "/Error/Dmp.ashx";
    return url;
}

std::string GetBreakpadUrl(const std::string &anyUrl, bool changeToDataDomain)
{
    std::string url = trim_trailing_slashes(anyUrl);
	if (changeToDataDomain)
		url = ReplaceTopSubdomain(url, "arl"); //url = ReplaceTopSubdomain(url, "data");

	url = url + "/Error/Breakpad.ashx";
    return url;
}

std::string ReplaceTopSubdomain(const std::string& anyUrl, const char* newTopSubDoman)
{
    std::string result(anyUrl);
    std::size_t foundPos = result.find("arl.");
    if (foundPos != std::string::npos)
    {
        result.replace(foundPos, 3, newTopSubDoman);
    }
    else if ((foundPos = result.find("m.")) != std::string::npos)
    {
        result.replace(foundPos, 1, newTopSubDoman);
    }
    return result;
}

std::string BuildGenericPersistenceUrl(const std::string& baseUrl, const std::string &servicePath)
{
    std::string constructedURLDomain(baseUrl);
    std::string constructedServicePath(servicePath);

	constructedURLDomain = ReplaceTopSubdomain(constructedURLDomain, "arl");
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
	
	constructedURLDomain = ReplaceTopSubdomain(constructedURLDomain, "arl");
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

