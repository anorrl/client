#include "stdafx.h"

#include "Util/ContentId.h"
#include "Util/Http.h"
#include "Util/Statistics.h"
#include <boost/functional/hash.hpp>
#include <boost/algorithm/string.hpp>

#include "util/LegacyContentTable.h"

#include "util/URL.h"

DYNAMIC_FASTFLAGVARIABLE(UrlReconstructRejectInvalidSchemes, false);

namespace
{			
	const char* kNamedUniverseAssetBase = "arlgameasset://";
	const char* kNamedUniverseAssetAssetNameParam = "assetName=";

	const char* kValidAssetPaths[] =
	{
		// assets
		"asset",
		"asset/",
		"asset/bodycolors.ashx",
		// thumbnails
		"thumbs/asset.ashx",
		"thumbs/avatar.ashx",
		"thumbs/script.png",
		"thumbs/staticimage",
		"game/tools/thumbnailasset.ashx",
		// scripts
		"game/edit.slua",
		"game/gameserver.slua",
		"game/join.ashx",
		"game/visit.slua"
	};

	void createIdUrl(std::string& result, const std::string& baseUrl, const std::string& id)
	{
		result.reserve(baseUrl.size() + id.size() + 16);
		
		result = baseUrl;
		
		// append slash only if baseUrl does not end with one
		if (result.empty() || result[result.size() - 1] != '/')
			result += '/';
		
		result += "asset/?id=";
		result += id;
	}
}


namespace ARL
{
    static boost::once_flag legacyContentTableFlag = BOOST_ONCE_INIT;
	static scoped_ptr<LegacyContentTable> legacyContentTable;

    static void initLegacyContentTable()
    {
        legacyContentTable.reset(new LegacyContentTable);
    }

	bool operator<(const ContentId& a, const ContentId& b) {
		return a.id < b.id;
	}
	bool operator!=(const ContentId& a, const ContentId& b) {
		return a.id != b.id;
	}
	bool operator==(const ContentId& a, const ContentId& b) {
		return a.id == b.id;
	}
	ContentId ContentId::fromUrl(const std::string& url) {
		// TODO: Throw if string isn't a real Url?
		return ContentId(url);
	}

	ContentId ContentId::fromGameAssetName(const std::string& gameAssetName)
	{
		return ContentId(kNamedUniverseAssetBase + gameAssetName);
	}

	void ContentId::CorrectBackslash(std::string& id)
	{
		for (size_t i = 0; i < id.size(); ++i)
			if (id[i] == '\\')
				id[i] = '/';
	}
	
	bool ContentId::isNamedAsset() const
	{
		return id.compare(0, strlen(kNamedUniverseAssetBase), kNamedUniverseAssetBase) == 0;
	}

	bool ContentId::isConvertedNamedAsset() const
	{
		return id.find(kNamedUniverseAssetAssetNameParam) != std::string::npos;
	}

	void ContentId::convertAssetId(const std::string& baseUrl, int universeId)
	{
		if(isAssetId())
			createIdUrl(id, baseUrl, id.substr(13));
		else if(isARLHttp()){
			id = baseUrl + id.substr(10);
		}
		else if (isNamedAsset())
		{
			ARLASSERT(!baseUrl.empty());
			if (!baseUrl.empty())
			{
				std::stringstream ss;
				ss << baseUrl;
				if (baseUrl[baseUrl.size() - 1] != '/')
				{
					ss << '/';
				}
				ss << "asset/"
					<< "?universeId=" << universeId
					<< "&" << kNamedUniverseAssetAssetNameParam << ARL::Http::urlEncode(id.substr(strlen(kNamedUniverseAssetBase)));

				id = ss.str();
			}
		}
	}
	
	void ContentId::convertToLegacyContent(const std::string& baseUrl)
	{	
		if (isAsset())
		{
            boost::call_once(initLegacyContentTable, legacyContentTableFlag);

			const std::string& mappedAssetId = legacyContentTable->FindEntry(id);
			
			if (!mappedAssetId.empty())
            {
                if (isdigit(mappedAssetId[0]))
                    createIdUrl(id, baseUrl, mappedAssetId);
                else
                    id = mappedAssetId;
            }
		}
	}

	bool ContentId::reconstructUrl(const std::string& baseUrl, const char* const paths[], const int pathCount)
	{
		boost::scoped_array<char> url(new char[id.length()+1]);

		// remove any spaces since HTParse stops parsing at the first space and we need to handle "http://www.anorrl.com/asset/?id= 1818"
		char* urlIter = url.get();
		for (size_t i = 0; i < id.length(); ++i)
		{
			if (id[i] != ' ')
			{
				*urlIter = id[i];
				urlIter++;
			}
		}
		*urlIter = '\0';
        
		const ARL::Url parsed = ARL::Url::fromString(url.get());

		// As of 2016-02-09 ContentId::id strings may have following forms:
		// - http://host/path?query -- a valid HTTP URL
		// - arlasset://something/file
		// - /asset/?query -- these should not be valid
		
		// Only HTTP URLs are subject for reconstruction
		if (!parsed.hasHttpScheme())
		{
			return !DFFlag::UrlReconstructRejectInvalidSchemes || parsed.hasValidScheme();
		}
		
		// Refuse to reconstruct malformed URLs
		if (!parsed.isValid() || parsed.pathIsEmpty())
		{
			return false;
		}
		
		// These can be rewritten
		std::string scheme = parsed.scheme();
		std::string host = parsed.host();
		std::string path = parsed.path();

		const ARL::Url baseUrlParsed = ARL::Url::fromString(baseUrl);

		for (int i = 0; i < pathCount; i++)
		{
			if (parsed.pathEqualsCaseInsensitive(paths[i]))
			{
				scheme = "http";
				host = "www.anorrl.com";
				
				id = ARL::Url::fromComponents(scheme, host, path, parsed.query(), parsed.fragment()).asString();

				return true;
			}
		} // for all paths[]
		
		return false;
	}

	bool ContentId::reconstructAssetUrl(const std::string& baseUrl)
	{
		return reconstructUrl(baseUrl, kValidAssetPaths, ARRAYSIZE(kValidAssetPaths));
	}

	std::string ContentId::getAssetId() const
	{
		if(isAssetId()){
			return id.substr(13); 
		}
		else if(isHttp() || isARLHttp()){
			std::string lower = id;
			std::transform(lower.begin(), lower.end(), lower.begin(), tolower);
			std::string::size_type pos = lower.find("id=");
			if(pos != std::string::npos){
				pos += 3;
				std::string::size_type second_pos = lower.find('&',pos);
				if(second_pos == std::string::npos){
					second_pos = lower.size();
				}
				return lower.substr(pos, (second_pos-pos));
			}
		}
		return "";
	}

	std::string ContentId::getAssetName() const
	{
		if (!isNamedAsset())
		{
			return "";
		}
		return id.substr(strlen(kNamedUniverseAssetBase));
	}

	std::string ContentId::getUnConvertedAssetName() const
	{
		if (!isConvertedNamedAsset())
		{
			return "";
		}
		std::string::size_type namedAssetParamPos = id.find(kNamedUniverseAssetAssetNameParam) +
			strlen(kNamedUniverseAssetAssetNameParam);
		std::string::size_type namedAssetParamEnd = id.find("&", namedAssetParamPos);
		std::string::size_type namedAssetParamLength = 
			namedAssetParamEnd == std::string::npos ?
				std::string::npos : namedAssetParamEnd - namedAssetParamPos;

		std::string urlEncodedNamedAsset = id.substr(namedAssetParamPos, namedAssetParamLength);
		return Http::urlDecode(urlEncodedNamedAsset);
	}

	ContentId ContentId::fromAssets(const char* filePath) {
		std::string header("arlasset://");
		return ContentId(header + filePath);
	}

	std::size_t hash_value(const ContentId& id)
	{
		boost::hash<std::string> hasher;
		return hasher(id.toString());
	}
}
