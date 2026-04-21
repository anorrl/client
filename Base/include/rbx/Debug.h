#pragma once

#include "RbxPlatform.h"
#include "RbxAssert.h"
#include "RbxFormat.h"
#include <set>
#include <ostream>
#include <fstream>
#include <assert.h>

#if (defined(_DEBUG) && defined(_WIN32))
#include <crtdbg.h>
#endif

#ifdef __ANDROID__
#include <typeinfo>
#include <cstdlib>
#endif

#ifdef _WIN32
#undef min
#undef max
#endif

#include "rbx/Declarations.h"
#include "FastLog.h"

#ifndef _WIN32
#define __noop
inline void DebugBreak()
{
#if defined(__i386__)
	// gcc on intel
		__asm__ __volatile__ ( "int $3" ); 
#else
	// some other gcc
	::abort();
#endif
}
#endif

LOGGROUP(Asserts)

/* Overview of builds and switches:

ARLASSERT:					Standard assert.  Should be reasonably fast.  Do not do "finds" or complex stuff here.  Simple bools, simple math, a couple levels of pointer indirection, etc.
ARLASSERT_VERY_FAST:		High fr  equency, extremely fast assert.  Not in regular debug build because frequency too high.  Mostly inner engine stuff
ARLASSERT_SLOW:				Put things like "find" here.  Will always run in debug builds
ARLASSERT_IF_VALIDATING:	Very slow stuff.  Only turns on if the "validating debug" switch is turned on in debug or noOpt build
ARLASSERT_FISHING:			Usually doesn't go off, should be safe - turn on for engine testing

							ARLASSERT()			ARLASSERT_VERY_FAST()	ARLASSERT_SLOW()		ARLASSERT_IF_VALIDATING()	ARLASSERT_FISHING()
	DEBUG						X					X						X							X							-
	NoOpt						X					X						-							-							-
	ReleaseAssert				X					-						-							-							-
	Release						-					-						-							-							-

*/


#ifdef _DEBUG
	#define __ARL_VERY_FAST_ASSERT
	#define __ARL_VALIDATE_ASSERT
//	#define __ARL_SLOW_ASSERT	// TODO: Hire a physics guy to enable them
//	#define __ARL_FISHING_ASSERT
	#define __ARL_NOT_RELEASE
#endif

#ifdef _NOOPT
	#define __ARL_CRASH_ON_ASSERT
	#define __ARL_VERY_FAST_ASSERT
	#define __ARL_NOT_RELEASE
#endif

namespace ARL {

	// Used for memory leak detection and other stuff
	class Debugable
    {
	public:
		// this is here as a last chance way to debug an assert build, force assertions on, but not crash
		static volatile bool doCrashEnabled;
        
		static void doCrash();
		static void doCrash(const char*);

		static void* badMemory() {return reinterpret_cast<void*>(0x00000003);}		// set values to this when deleting to check if ever coming back
	};

}

void ARLCRASH();
void ARLCRASH(const char* message);

void ReleaseAssert(int channel, const char* msg);

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

// macro to convince a compiler a variable is used while not generating instructions (useful for removing warnings)
#define ARL_UNUSED(x) (void)(sizeof((x), 0))

// This macro will cause a crash. Usually you don't call it directly. Use ARLASSERT instead
#define ARL_CRASH_ASSERT(expr) \
		((void) (!!(expr) || \
		((ARL::_internal::_debugHook != NULL) && (ARL::_internal::_debugHook(#expr, __FILE__, __LINE__))) || \
		(ARL::Debugable::doCrash(#expr), 0)))

// This macro will just log an assert string, if we will run into crash log with the assert information will be sent to us
#define ARL_LOG_ASSERT(expr) \
	((void) (FLog::Asserts && (!!(expr) || \
	((ARL::_internal::_debugHook != NULL) && (ARL::_internal::_debugHook(#expr, __FILE__, __LINE__))) || \
	(ReleaseAssert(FLog::Asserts,#expr " file: " __FILE__ " line: " TOSTRING(__LINE__)), 0))))


// LEGACY_ASSERT should be used when we have some assert bogging us and it seems like this guy is a good candidate for removal
// usage just replace ARLASSERT with LEGACY_ASSERT and it will gone by default, but if you need to see it temporary define FIRE_LEGACY_ASSERT
#undef FIRE_LEGACY_ASSERT

#ifdef FIRE_LEGACY_ASSERT
	#define LEGACY_ASSERT(expr) ARLASSERT(expr)
#else
	#define LEGACY_ASSERT(expr) ((void)0)
#endif

#define ARLASSERTENABLED

// ARLASSERT()
//
#ifdef __ARL_CRASH_ON_ASSERT
	#define ARLASSERT ARL_CRASH_ASSERT
#else
	#if (defined(_DEBUG) && defined(__APPLE__))		// Apple Debug
        #include "TargetConditionals.h"
        #if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
            #define ARLASSERT ARL_LOG_ASSERT // iOS has no way to step over asserts (makes debugging hard)
        #else
            #define ARLASSERT(expr) assert(expr)
            #define ARLASSERTENABLED
        #endif
	#elif (defined(_DEBUG) && defined(_WIN32))		// Windows Debug
		#define ARLASSERT(expr) \
			((void) (!!(expr) || \
			((ARL::_internal::_debugHook != NULL) && (ARL::_internal::_debugHook(#expr, __FILE__, __LINE__))) || \
			(_ASSERTE(expr), 0)))
		#define ARLASSERTENABLED
	#else											// All Platform Release
		#define ARLASSERT ARL_LOG_ASSERT
	#endif
#endif



// ARLASSERT_VERY_FAST()
//
#ifdef __ARL_VERY_FAST_ASSERT
	#define ARLASSERT_VERY_FAST(expr) ARLASSERT(expr)
#else
	#define ARLASSERT_VERY_FAST(expr) ((void)0)
#endif


// ARLASSERT_SLOW()
//
#ifdef __ARL_SLOW_ASSERT
	#define ARLASSERT_SLOW(expr) ARLASSERT(expr)
#else
	#define ARLASSERT_SLOW(expr) ((void)0)
#endif


// ARLASSERT_FISHING)
//
#ifdef __ARL_FISHING_ASSERT
	#define ARLASSERT_FISHING(expr) ARLASSERT(expr)
#else
	#define ARLASSERT_FISHING(expr) ((void)0)
#endif


// ARLASSERT_IF_VALIDATING()
//
#ifdef __ARL_VALIDATE_ASSERT
	#define ARLASSERT_IF_VALIDATING(expr)	ARLASSERT( (expr) ) 

#else
	#define ARLASSERT_IF_VALIDATING(expr) ((void)0)
#endif


// ARLASSERT_NOT_RELEASE()		make sure this code is not being compiled in release build
#ifdef __ARL_NOT_RELEASE
	#define ARLASSERT_NOT_RELEASE()			((void)0)
#else
	#define ARLASSERT_NOT_RELEASE()			ARLCRASH()
#endif


// Same as boost::polymorphic_downcast but with an ARLASSERT
template<class T, class U>
inline T rbx_static_cast(U u) {
	ARLASSERT_SLOW(dynamic_cast<T>(u)==u);
	return static_cast<T>(u);
}
