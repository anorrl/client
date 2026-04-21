#include "stdafx.h"

#include "Script/LuaSettings.h"
#include "LuaConf.h"

const char *const ARL::sLuaSettings = "LuaSettings";

REFLECTION_BEGIN();
static ARL::Reflection::BoundProp<int> prop_1("GcPause", "Garbage Collection", &ARL::LuaSettings::gcPause);
static ARL::Reflection::BoundProp<int> prop_2("GcStepMul", "Garbage Collection", &ARL::LuaSettings::gcStepMul);
static ARL::Reflection::BoundProp<double> prop_3("DefaultWaitTime", "Settings", &ARL::LuaSettings::defaultWaitTime);
static ARL::Reflection::BoundProp<int> prop_4("GcLimit", "Garbage Collection", &ARL::LuaSettings::gcLimit);
static ARL::Reflection::BoundProp<int> prop_5("GcFrequency", "Garbage Collection", &ARL::LuaSettings::gcFrequency);
static ARL::Reflection::BoundProp<bool> prop_areScriptStartsReported("AreScriptStartsReported", "Diagnostics", &ARL::LuaSettings::areScriptStartsReported);
static ARL::Reflection::BoundProp<float> prop_AreWaitingThreadsBudgeted("WaitingThreadsBudget", "Settings", &ARL::LuaSettings::waitingThreadsBudget);
REFLECTION_END();

ARL::LuaSettings::LuaSettings(void)
	:gcPause(LUAI_GCPAUSE)
	,gcStepMul(LUAI_GCMUL)
	,defaultWaitTime(0.03)
    ,smallestWaitTime(0.016667)
	,gcLimit(2)
	,gcFrequency(0)
	,areScriptStartsReported(false)
	,waitingThreadsBudget(0.1)
{
	setName("Lua");
}
