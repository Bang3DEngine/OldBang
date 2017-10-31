#include "Bang/Time.h"

#include <chrono>

#include "Bang/Application.h"

USING_NAMESPACE_BANG

double Time::time      = 0.0;
float  Time::deltaTime = 0.0f;

float Time::GetDeltaTime()
{
    return Time::GetInstance()->m_deltaTime;
}

double Time::GetTime()
{
    return Time::GetInstance()->m_time;
}

unsigned long long Time::GetNow()
{
    return std::chrono::system_clock::now().time_since_epoch() /
           std::chrono::milliseconds(1);
}

Time::Time()
{
}

void Time::OnFrameStarted()
{
    float deltaTime = (Time::GetNow() - m_lastRenderTime) / 1000.0f;
    m_deltaTime = deltaTime;
    m_time += deltaTime;

    Time::deltaTime = m_deltaTime;
    Time::time = m_time;
}

void Time::OnFrameFinished()
{
    m_lastRenderTime = Time::GetNow();
}

void Time::ResetDeltaTime()
{
    Time::GetInstance()->m_lastRenderTime = Time::GetNow();
}

Time *Time::GetInstance()
{
    return Application::GetInstance()->GetTime();
}
