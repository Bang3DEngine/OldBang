#include "Time.h"

#include <chrono>

#include "SingletonManager.h"

double Time::s_time = 0.0f;
double const& Time::time = Time::s_time;
float Time::s_deltaTime = 0.0f;
float const& Time::deltaTime = Time::s_deltaTime;

void Time::InitFromMainBinary()
{
   SingletonManager::Set<Time>(new Time());
}

Time* Time::GetInstance()
{
    return SingletonManager::Get<Time>();
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
