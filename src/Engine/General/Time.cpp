#include "Bang/Time.h"

#include <chrono>

#include "Bang/Application.h"

USING_NAMESPACE_BANG

float Time::GetDeltaTime()
{
    Time *time = Time::GetInstance();
    return (Time::GetNow() - time->m_deltaTimeReference) / 1000.0f;
}

double Time::GetTime()
{
    return Time::GetNow() / 1000.0f;
}

uint64_t Time::GetNow()
{
    return std::chrono::system_clock::now().time_since_epoch() /
           std::chrono::nanoseconds(1);
}

Time::Time()
{
}

void Time::EstablishDeltaTimeReferenceToNow()
{
    m_deltaTimeReference = Time::GetNow();
}

Time *Time::GetInstance()
{
    return Application::GetInstance()->GetTime();
}
