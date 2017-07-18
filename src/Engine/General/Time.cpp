#include "Bang/Time.h"

#include <chrono>

#include "Bang/Application.h"

double        Time::s_time           = 0.0;
float         Time::s_deltaTime      = 0.0f;
double        Time::s_lastRenderTime = 0.0f;
double const& Time::time             = Time::s_time;
float  const& Time::deltaTime        = Time::s_deltaTime;

void Time::OnFrameStarted()
{
    float deltaTime = float(Time::GetNow() - s_lastRenderTime) / 1000.0f;
    Time::s_deltaTime = deltaTime;
    Time::s_time += deltaTime;
}

void Time::OnFrameFinished()
{
    Time::s_lastRenderTime = Time::GetNow();
}

float Time::GetDeltaTime()
{
    return Time::deltaTime;
}

double Time::GetTime()
{
    return Time::time;
}

void Time::ResetDeltaTime()
{
    Time::s_lastRenderTime = Time::GetNow();
}

unsigned long long Time::GetNow()
{
    return std::chrono::system_clock::now().time_since_epoch() /
           std::chrono::milliseconds(1);
}
