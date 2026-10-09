// This prevent inclusion of winsock.h in windows.h, which prevents windows redifinition errors
// Look at winsock2.h for details, winsock2.h is #included from boost.hpp & other places.
#ifdef _WIN32
#define _WINSOCKAPI_
#endif

#include "arl/rbxTime.h"
#include "arl/Debug.h"
#include "arl/atomic.h"
#include <stdexcept>
#include "FastLog.h"

#ifdef __APPLE__
#include <mach/mach.h>
#include <mach/mach_time.h>
#endif

#if defined(_WIN32)
#include <windows.h>
#include "Mmsystem.h"
#pragma comment (lib, "Winmm.lib")
#else
#include <unistd.h>
#endif
#include <thread>
#include <chrono>

FASTINTVARIABLE(SpeedTestPeriodMillis, 1000)
FASTINTVARIABLE(MaxSpeedDeltaMillis, 300)
FASTINTVARIABLE(SpeedCountCap, 5)

namespace ARL
{

#if defined(_WIN32)
static volatile double currentSeconds = 0;
static volatile bool cheater = false;
static volatile bool isDebuggedValue = false;

static const int correctionsPerSecond = 10; // used for interpolation
static const double secondsPerCorrection = 1.0 / correctionsPerSecond;
static double millisecondsPerTick;  // The nominal seconds per tick requested of the timer
static long ticksPerCorrection;      // How many ticks should go by between correction

// stuff to prevent GameCheat Speed hack
static long lastSpeedCheckCounter = 0;

static long violationsCount = 0;

LARGE_INTEGER prevSpeedHackCheckpointTime;
long prevTickTime = 0;

static LARGE_INTEGER getSysTime()
{
    SYSTEMTIME stime;
    GetSystemTime(&stime);
    FILETIME ftime;
    SystemTimeToFileTime(&stime, &ftime);

    LARGE_INTEGER r;
    r.HighPart = ftime.dwHighDateTime;
    r.LowPart = ftime.dwLowDateTime;

    return r;
}

static void checkSpeedHack()
{
    lastSpeedCheckCounter ++;
    if (lastSpeedCheckCounter == FInt::SpeedTestPeriodMillis)
    {
        LARGE_INTEGER curTime = getSysTime();

        LARGE_INTEGER delta;
        delta.QuadPart = curTime.QuadPart - prevSpeedHackCheckpointTime.QuadPart;
        
        FILETIME ftime;
        ftime.dwHighDateTime = delta.HighPart;
        ftime.dwLowDateTime = delta.LowPart;
        
        SYSTEMTIME stime;
        FileTimeToSystemTime(&ftime, &stime);

        long curTickTime = timeGetTime();

        long td = curTickTime - prevTickTime;
        long sd = 1000*stime.wSecond + stime.wMilliseconds;
        if (abs(td - sd) > FInt::MaxSpeedDeltaMillis)
        {
            violationsCount ++;
        }
        else
        {
            violationsCount = 0;
        }

        if (violationsCount >= FInt::SpeedCountCap)
        {
            cheater = true;
        }

        prevSpeedHackCheckpointTime.QuadPart = curTime.QuadPart;
        prevTickTime = curTickTime;
        lastSpeedCheckCounter = 0;
    }
}

static void checkDbg()
{
    return;
}

static arl::atomic<int> recguard;
#endif
static double tick_frequency_helper()
{
#if defined(_WIN32)
    LARGE_INTEGER tickFreq;
    int rval = QueryPerformanceFrequency(&tickFreq);
    ARLASSERT(rval!=0);
    return 1.0 / static_cast<long long>(tickFreq.QuadPart);
#elif defined(__APPLE__)
    kern_return_t kerror;
    mach_timebase_info_data_t tinfo;
    kerror = mach_timebase_info(&tinfo);
    if (kerror != KERN_SUCCESS){
        tinfo.denom = 1;
        tinfo.numer = 1;
    }
    return static_cast<double>(tinfo.numer/(double)tinfo.denom * 1e-9);

#elif defined(__ANDROID__) || defined(__linux__)
    return 1e-9;
    
#endif
}
static double tick_resolution(){
    static const double tick_res = tick_frequency_helper();
    return tick_res;

}
long long Time::getTickCount(){
#if defined(_WIN32)
    LARGE_INTEGER ticks;
    int rval = QueryPerformanceCounter(&ticks);
    ARLASSERT(rval!=0);
    return ticks.QuadPart;
#elif defined(__APPLE__)
    return mach_absolute_time();
#elif defined(__ANDROID__) || defined(__linux__)
    timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return static_cast<long long>(now.tv_sec) * 1000000000LL + now.tv_nsec;
#else
    timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return static_cast<long long>(now.tv_sec) * 1000000000LL + now.tv_nsec;
#endif
}
long long Time::getStart()
{
    // not worried about potential multi-threaded double-init.
    // assumptions: underlying type is long long.
    static const long long start = getTickCount();
    return start;
}

#if defined(_WIN32)

void CALLBACK directCallback(UINT, UINT, DWORD, DWORD, DWORD) 
{ 
    if(recguard.swap(1) == 1)
    {
        return; // we are re-entering, ignore this call.
    }

    double t = (Time::getTickCount() - Time::getStart())*tick_resolution();
    if (t>currentSeconds)
        currentSeconds = t;

    checkSpeedHack();
    checkDbg();

    recguard.swap(0);
} 

void CALLBACK interpolatedCallback(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2) 
{ 
    if(recguard.swap(1) == 1)
    {
        return; // we are re-entering, ignore this call.
    }

    static double last = Time::getStart()*tick_resolution();
    static long mmticks = 0;
    static double secsPerTick = millisecondsPerTick / 1000.0;

    int ticksSinceLastCorrection = mmticks % ticksPerCorrection;

    double t;
    if (ticksSinceLastCorrection == 0)
    {
        double current = Time::getTickCount()*tick_resolution();
        
        double elapsedTime = (current - last);
        // Make sure that elapsedTime is reasonable. 
        // If it isn't then we may have a bad sample.
        // For example, the thread could have been suspended for some reason
        if (elapsedTime > 1.05 * secondsPerCorrection)
            {}
        else if (elapsedTime < 0.95 * secondsPerCorrection)
            {}
        else
            secsPerTick = elapsedTime / (double) ticksPerCorrection;

        last = current;
        t = current - Time::getStart()*tick_resolution();
    }
    else
    {
        t = last - Time::getStart()*tick_resolution() + secsPerTick * (double) ticksSinceLastCorrection;
    }


    if (t>currentSeconds)
        currentSeconds = t;

    mmticks++;

    checkSpeedHack();
    checkDbg();

    recguard.swap(0);
} 

void startMMTimer()
{
    const int targetResolution = 1;      // 1-millisecond target resolution

    TIMECAPS tc;
    if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) != TIMERR_NOERROR) 
        throw std::runtime_error("Failed timeGetDevCaps");

    UINT timerRes = std::min<UINT>(std::max<UINT>(tc.wPeriodMin, targetResolution), tc.wPeriodMax);
    
    millisecondsPerTick = timerRes;
    ticksPerCorrection = 1000 / (correctionsPerSecond * timerRes);

    prevSpeedHackCheckpointTime = getSysTime();
    prevTickTime = timeGetTime();

    timeBeginPeriod(timerRes); 

    MMRESULT result = timeSetEvent(
        timerRes, // delay
        timerRes, // resolution (global variable)
        ticksPerCorrection >= 2
                ? reinterpret_cast<void (*)(UINT, UINT, DWORD_PTR, DWORD_PTR, DWORD_PTR)>(interpolatedCallback)
                : reinterpret_cast<void (*)(UINT, UINT, DWORD_PTR, DWORD_PTR, DWORD_PTR)>(directCallback),
        NULL, // user data
        TIME_PERIODIC );  // single timer event

    if (!result)
        throw std::runtime_error("Failed timeSetEvent");
}
    
#endif

Time::SampleMethod Time::preciseOverride = Time::Precise;

template<>
Time Time::now<Time::Precise>()
{
    Time result;
    result.sec = (getTickCount() - getStart())*tick_resolution();
    return result;
}


template<>
Time Time::now<Time::Multimedia>()
{
#if defined(_WIN32)
    return Time(timeGetTime() / 1000.0);
#else
    return now<Time::Precise>();
#endif
}

bool Time::isSpeedCheater()
{
#if defined(_WIN32)
    return cheater;
#else
    return false;
#endif
}

bool Time::isDebugged()
{
#if defined(_WIN32)
    return isDebuggedValue;
#else
    return false;
#endif
}

template<>
Time Time::now<Time::Fast>()
{
#if defined(_WIN32)
    if (preciseOverride <= Fast)
        return now<Precise>();
    
    static arl::atomic<int> inited;
    if (inited==0)
        if (inited.swap(1) == 0)
        {
            startMMTimer();
            FLog::Init(nowFastSec);
        }
    
    Time result;
    result.sec = currentSeconds;
    return result;
#else
    return now<Time::Precise>();
#endif
}

Time Time::nowFast()
{
     return now<Time::Fast>();
}

double Time::nowFastSec()
{
    return Time::nowFast().timestampSeconds();
}

template<>
Time Time::now<Time::Benchmark>()
{
    if (preciseOverride <= Benchmark)
        return now<Precise>();
    else
        return now<Fast>();
}

Time Time::now(SampleMethod sampleMethod)
{
    switch (sampleMethod)
    {
    default:
    case Fast:
        return now<Fast>();

    case Precise:
        return now<Precise>();

    case Benchmark:
        return now<Benchmark>();

    case Multimedia:
        return now<Multimedia>();
    }
}

Time::Interval operator-( const Time& t1, const Time& t0 )
{
    const double seconds = t1.sec - t0.sec;
    return Time::Interval(seconds);
}

void Time::Interval::sleep()
{
#if defined(_WIN32)
    Sleep(static_cast<DWORD>(sec * 1e3));
#else
    std::this_thread::sleep_for(std::chrono::microseconds(static_cast<long long>(sec * 1e6)));
#endif

}

}