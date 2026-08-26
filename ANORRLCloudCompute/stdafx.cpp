// stdafx.cpp : source file that includes just the standard includes
// ACCService.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

// stupid fucking hack to get curl to compile since these functions were like rewritten in CRT
// https://stackoverflow.com/a/32449318
FILE _iob[] = { *stdin, *stdout, *stderr };
extern "C" FILE * __cdecl __iob_func(void) { return _iob; }