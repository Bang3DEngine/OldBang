#ifndef TIME_H
#define TIME_H

#include "Bang/Bang.h"

NAMESPACE_BANG_BEGIN

class Time
{
public:
    static float GetDeltaTime();
    static double GetNow_Seconds();
    static uint64_t GetNow_Millis();
    static uint64_t GetNow_Nanos();
    static void SetDeltaTime(double seconds);
    static void SetDeltaTimeReferenceToNow();

private:
    double m_time = 0.0;
    float m_deltaTime = 0.0f;
    uint64_t m_deltaTimeReference = 0.0;

    Time() = default;
    virtual ~Time() = default;

    static Time *GetInstance();

    friend class Application;
};

NAMESPACE_BANG_END

#endif // TIME_H
