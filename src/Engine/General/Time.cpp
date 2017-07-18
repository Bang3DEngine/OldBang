#include "Bang/Time.h"

#include <chrono>

#include "Bang/Application.h"
#include "Bang/SingletonManager.h"

double Time::s_time = 0.0f;
double const& Time::time = Time::s_time;
float Time::s_deltaTime = 0.0f;
float const& Time::deltaTime = Time::s_deltaTime;

Time* Time::GetInstance()
{
    return Application::GetInstance()->GetTime();
}

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
