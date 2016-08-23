#ifndef TIME_H
#define TIME_H

#include "Bang.h"

#include <chrono>

class Time
{
friend class Behaviour;
friend class GameObject;
friend class Application;
friend class SceneManager;

private:
    float m_deltaTime = 0.0f;
    static float zero;

    Time() {}
    static Time* GetInstance();

public:

    static float s_deltaTime;
    static float const& deltaTime;

    static void InitFromMainBinary();

    /**
     * @brief Returns the number of seconds that have passed between
     *        the last frame and the current one (the delta time)
     * @return The delta time in seconds.
     */
    static float GetDeltaTime();

    /**
     * @brief GetNow
     * @return Returns the epoch time.
     */
    static unsigned long long GetNow();
};

#endif // TIME_H
