#pragma once

#include "RBX/Debug.h"

// Engine assertions often cause the game to stop running.
// Until we can fix these, turn them off.
//#define ARL_DEBUGENGINE

#ifdef ARL_DEBUGENGINE
#define ARL_ENGINE_ASSERT(expr) ARLASSERT(expr)
#else
#define ARL_ENGINE_ASSERT(expr) ((void)0)
#endif