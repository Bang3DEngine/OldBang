#include "Time.h"

float Time::deltaTime = 0.0f;

float Time::GetDeltaTime()
{
    return deltaTime;
}

unsigned long long Time::GetNow()
{
    return std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
}
