#include "Timer.h"

Timer::Timer(std::function<void ()> func, int msDelay, int repeatCount, bool waitOneDelayTime) :
    func(func),
    msDelay(msDelay),
    repeatCount(repeatCount),
    waitOneDelayTime(waitOneDelayTime),
    stop(true)
{
    timerThread = std::thread( std::bind(&Timer::TimerLoop, this) );
}

void Timer::TimerLoop() const
{
    int count = 0;
    while(stop);

    if(!waitOneDelayTime) { ++count; func(); }

    while( !stop && (repeatCount == -1 || count < repeatCount) )
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(msDelay));
        func();
        ++count;
    }
}

void Timer::Start() const
{
    stop = false;
}

void Timer::Stop() const
{
    stop = true;
}
