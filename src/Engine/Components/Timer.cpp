#include "Bang/Timer.h"

USING_NAMESPACE_BANG

void Timer::Run()
{
    ResetTimestamp();
    m_running = true;
}

void Timer::Stop()
{
    m_running = false;
}

void Timer::OnUpdate()
{
    Component::OnUpdate();

    if (IsRunning())
    {
        float secsSinceLastTick =
                            (Time::GetNow_Seconds() - m_lastTickTimestampSecs);
        if (secsSinceLastTick > GetInterval())
        {
            Tick();
        }
    }
}

void Timer::Tick()
{
    for (auto callback : m_callbacksList) { callback(); }
    ResetTimestamp();
}

void Timer::ResetTimestamp()
{
    m_lastTickTimestampSecs = Time::GetNow_Seconds();
}

void Timer::AddCallback(std::function<void ()> callback)
{
    m_callbacksList.PushBack(callback);
}

void Timer::SetInterval(float intervalSecs)
{
    m_intervalSecs = intervalSecs;
}

bool Timer::IsRunning() const { return m_running; }
float Timer::GetInterval() const { return m_intervalSecs; }

Timer::Timer()
{
}

Timer::~Timer()
{
}

