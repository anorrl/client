#pragma once

#if defined(_WIN32) && !defined(ARL_STUDIO_BUILD) && !defined(ARL_PLATFORM_DURANGO)
#include "Security/RandomConstant.h"

// This is junk code generation.  All of this is intended to be inlined
// and generally do nothing.

// there are 15*17 different seeds from this.
// (ARL_BUILDSEED%15 + 1) is between 1 and 16, and is coprime to 17.
#define ARL_JUNK (junk< ((ARL_BUILDSEED%15 + 1)*__LINE__ + ARL_BUILDSEED) % 17>())

template <int N> inline void junk() {}

#define ARL_NOP0 __asm _emit 0x0f __asm _emit 0x1f __asm _emit 0x00
#define ARL_NOP1 __asm _emit 0x8d __asm _emit 0x76 __asm _emit 0x00
#define ARL_NOP2 __asm _emit 0x0f __asm _emit 0x1f __asm _emit 0x40 __asm _emit 0x00
#define ARL_NOP3 __asm _emit 0x8d __asm _emit 0x74 __asm _emit 0x26 __asm _emit 0x00
#define ARL_NOP4 __asm _emit 0x90 __asm _emit 0x8d __asm _emit 0x74 __asm _emit 0x26 __asm _emit 0x00
#define ARL_NOP5 __asm _emit 0x66 __asm _emit 0x0f __asm _emit 0x1f __asm _emit 0x44 __asm _emit 0x00 __asm _emit 0x00
#define ARL_NOP6 __asm _emit 0x8d __asm _emit 0xb6 __asm _emit 0x00 __asm _emit 0x00 __asm _emit 0x00 __asm _emit 0x00
template<> inline void junk<0>()
{
    _asm
    {
        ARL_NOP0;
    }
}


template<> inline void junk<1>()
{
    _asm
    {
        ARL_NOP1;
    }
}

template<> inline void junk<2>()
{
    _asm
    {
        ARL_NOP2;
    }
}

template<> inline void junk<3>()
{
    _asm
    {
        ARL_NOP3;
    }
}

template<> inline void junk<4>()
{
    _asm
    {
        ARL_NOP4;
    }
}

template<> inline void junk<5>()
{
    _asm
    {
        ARL_NOP5;
    }
}

template<> inline void junk<6>()
{
    _asm
    {
        ARL_NOP6;
    }
}

template<> inline void junk<7>()
{
    _asm
    {
        ARL_NOP0;
        ARL_NOP1;
    }
}

template<> inline void junk<8>()
{
    _asm
    {
        ARL_NOP1;
        ARL_NOP2;
    }
}

template<> inline void junk<9>()
{
    _asm
    {
        ARL_NOP1;
        ARL_NOP3;   
    }
}

template<> inline void junk<10>()
{
    _asm
    {
        ARL_NOP4;
        ARL_NOP0;
    }
}

template<> inline void junk<11>()
{
    _asm
    {
        ARL_NOP3;
        ARL_NOP5;
    }
}

template<> inline void junk<12>()
{
    _asm
    {
        ARL_NOP2;
        ARL_NOP5;
    }
}

template<> inline void junk<13>()
{
    _asm
    {
        ARL_NOP6;
        ARL_NOP0;
    }
}

template<> inline void junk<14>()
{
    _asm
    {
        ARL_NOP2;
        ARL_NOP1;
    }
}

template<> inline void junk<15>()
{
    _asm
    {
        ARL_NOP4;
        ARL_NOP5;
    }
}

template<> inline void junk<16>()
{
    _asm
    {
        ARL_NOP3;
        ARL_NOP2;
    }
}
#else
#define ARL_JUNK
#endif
