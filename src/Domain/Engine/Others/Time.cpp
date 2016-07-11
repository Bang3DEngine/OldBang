#include "Time.h"

#include "SingletonManager.h"

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
