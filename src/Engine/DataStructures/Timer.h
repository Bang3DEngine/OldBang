#ifndef TIMER_H
#define TIMER_H

#include <thread>
#include <functional>

class Timer
{
private:
    std::function<void()> m_func;
    float m_secDelay;
    int m_repeatCount;
    bool m_waitOneDelayTime;
    bool m_killed = false;

    mutable bool m_stop = true;

    std::thread m_timerThread;

    void TimerLoop() const;

    virtual ~Timer() {}   // Use Kill instead

public:
    Timer(std::function<void ()> func, float secDelay, int repeatCount = -1, bool waitOneDelayTime = true);

    void Start() const;
    void Stop() const;
    void Kill();
};

#endif // TIMER_H
