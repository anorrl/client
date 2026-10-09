#include "stdafx.h"

#include "util/ScriptInformationProvider.h"
#include "util/Http.h"
#include <sstream>

namespace ARL
{
const char* const sScriptInformationProvider = "ScriptInformationProvider";

REFLECTION_BEGIN();
static Reflection::BoundFuncDesc<ScriptInformationProvider, void(std::string)> func_setAssetUrl( &ScriptInformationProvider::setAssetUrl, "SetAssetUrl", "url", Security::LocalUser);
static Reflection::BoundFuncDesc<ScriptInformationProvider, void(std::string)> func_setAccessKey( &ScriptInformationProvider::setAccessKey, "SetAccessKey", "access", Security::ANORRL);
REFLECTION_END();

ScriptInformationProvider::ScriptInformationProvider()
	:DescribedNonCreatable<ScriptInformationProvider,Instance,sScriptInformationProvider>()
{
}

}//namespace

