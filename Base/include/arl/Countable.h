#pragma once
#include "arl/atomic.h"
#include "arl/Declarations.h"

namespace ARL
{

namespace Diagnostics
{

template<typename T>
class ARLBaseClass Countable
{
    static arl::atomic<int> count;
public:
    static long getCount() { return count; }
    ~Countable()
    {
        --count;
    }
protected:
    Countable()
    {
        ++count;
    }
};

template<class T>
arl::atomic<int> Countable<T>::count;

} // namespace Diagnostics

} // namespace ARL
