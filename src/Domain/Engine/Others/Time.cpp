#include "Time.h"

#include <chrono>

#include "SingletonManager.h"

float Time::s_deltaTime = 0.0f;
float const& Time::deltaTime = Time::s_deltaTime;

void Time::InitFromMainBinary()
{
   SingletonManager::GetInstance()->SetTimeSingleton(new Time());
}

Time* Time::GetInstance()
{
    return SingletonManager::GetInstance()->GetTimeSingleton();
}

float Time::GetDeltaTime()
{
    return Time::GetInstance()->m_deltaTime;
}

unsigned long long Time::GetNow()
{
    return std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
}
