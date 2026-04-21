/**
 @file debugAssert.cpp

 Windows implementation of assertion routines.

 @maintainer Morgan McGuire, graphics3d.com
 
 @created 2001-08-26
 @edited  2006-02-02
 */

#include "RbxPlatform.h" // includes <windows.h>

#include "RbxAssert.h"
#include "RbxFormat.h"
#include <string>

using namespace std;

namespace ARL {

namespace _internal {
    AssertionHook _debugHook;
    AssertionHook _failureHook;
} // internal
 
void setAssertionHook(AssertionHook hook) {
	ARL::_internal::_debugHook = hook;
}

AssertionHook assertionHook() {
	return 	ARL::_internal::_debugHook;
}

void setFailureHook(AssertionHook hook) {
	ARL::_internal::_failureHook = hook;
}

AssertionHook failureHook() {
	return ARL::_internal::_failureHook;
}


} // namespace

