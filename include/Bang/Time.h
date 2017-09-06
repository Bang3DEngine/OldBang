#ifndef TIME_H
#define TIME_H

#include "Bang/Bang.h"

NAMESPACE_BANG_BEGIN

class Time
{
public:

    static double time;
    static float deltaTime;

    /**
     * @brief Returns the number of seconds that have passed between
     *        the last frame and the current one (the delta time)
     * @return The delta time in seconds.
     */
    static float GetDeltaTime();
    static double GetTime();

    static void ResetDeltaTime();
    static unsigned long long GetNow();

private:
    double m_time = 0.0;
    float m_deltaTime = 0.0f;
    unsigned long long m_lastRenderTime = 0.0;

    Time();

    void OnFrameStarted();
    void OnFrameFinished();
    static Time *GetInstance();

    friend class Application;
};

NAMESPACE_BANG_END

#endif // TIME_H
