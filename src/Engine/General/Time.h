#ifndef TIME_H
#define TIME_H

class Time
{
public:
    static double s_time;
    static double const& time;
    static float s_deltaTime;
    static float const& deltaTime;
    static double s_lastRenderTime;

    static void OnFrameStarted();
    static void OnFrameFinished();

    /**
     * @brief Returns the number of seconds that have passed between
     *        the last frame and the current one (the delta time)
     * @return The delta time in seconds.
     */
    static float GetDeltaTime();
    static double GetTime();
    static void ResetDeltaTime();

    /**
     * @brief GetNow
     * @return Returns the epoch time in ms.
     */
    static unsigned long long GetNow();

private:
    Time() = delete;
};

#endif // TIME_H
