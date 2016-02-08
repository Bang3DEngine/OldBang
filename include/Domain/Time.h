#ifndef TIME_H
#define TIME_H

#include <chrono>

class Time
{
friend class Canvas;

private:
    static float deltaTime;

    Time() {}

public:
    static float GetDeltaTime();
    static unsigned long long GetNow();
};

#endif // TIME_H
