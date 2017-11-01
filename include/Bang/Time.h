#ifndef TIME_H
#define TIME_H

#include "Bang/Bang.h"

NAMESPACE_BANG_BEGIN

class Time
{
public:
    static float GetDeltaTime();
    static double GetTime();
    static unsigned long long GetNow();

private:
    double m_time = 0.0;
    float m_deltaTime = 0.0f;
    unsigned long long m_deltaTimeReference = 0.0;

    Time();

    void EstablishDeltaTimeReferenceToNow();
    static Time *GetInstance();

    friend class Application;
};

NAMESPACE_BANG_END

#endif // TIME_H
