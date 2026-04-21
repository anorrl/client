#pragma once

#ifdef _WIN32
#include <windows.h>

namespace ARL
{
class ScopedNamedMutex
{
    HANDLE hMutex;

public:
    ScopedNamedMutex(const char* name);
    ~ScopedNamedMutex();
};
} // namespace ARL
#endif // #ifdef _WIN32 