#pragma once

#include "script/script.h"
#include "script/ModuleScript.h"
#include "script/LuaSourceContainer.h"
#include "util/ContentId.h"
#include "util/HttpAsync.h"
#include "util/ProtectedString.h"


class LuaSourceBuffer
{

public:
	static LuaSourceBuffer fromInstance(boost::shared_ptr<ARL::Instance> instance);
	static LuaSourceBuffer fromContentId(const ARL::ContentId& contentId);
	static ARL::HttpFuture getScriptAssetSource(const ARL::ContentId& contentId, int universeId);

	LuaSourceBuffer();

	boost::shared_ptr<ARL::LuaSourceContainer> toInstance() const;

	bool isNamedAsset() const;
	bool isModuleScript() const;
	void reloadLiveScript();

	bool isCodeEmbedded() const;
	bool sourceAvailable() const;
	std::string getScriptText() const;
	void setScriptText(const std::string& newText);
	std::string getScriptName() const;
	std::string getFullName() const;

	bool empty() const;

	bool operator==(const LuaSourceBuffer& other) const;
	bool operator!=(const LuaSourceBuffer& other) const;

	inline size_t hash_value() const
	{
		return boost::hash_value(script) ^
			boost::hash_value(moduleScript) ^
			boost::hash_value(contentId.toString());
	}

private:
	boost::shared_ptr<ARL::Script> script;
	boost::shared_ptr<ARL::ModuleScript> moduleScript;
	ARL::ContentId contentId;

	explicit LuaSourceBuffer(boost::shared_ptr<ARL::ModuleScript> moduleScript);
	explicit LuaSourceBuffer(boost::shared_ptr<ARL::Script> script);
	explicit LuaSourceBuffer(const ARL::ContentId& contentId);
};

inline size_t hash_value(const LuaSourceBuffer& lsb)
{
	return lsb.hash_value();
}
