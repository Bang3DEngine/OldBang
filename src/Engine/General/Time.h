#ifndef TIME_H
#define TIME_H

class Time
{
public:
    static double s_time;
    static double const& time;
    static float s_deltaTime;
    static float const& deltaTime;

    static void InitFromMainBinary();

    /**
     * @brief Returns the number of seconds that have passed between
     *        the last frame and the current one (the delta time)
     * @return The delta time in seconds.
     */
    static float GetDeltaTime();

    static double GetTime();

    /**
     * @brief GetNow
     * @return Returns the epoch time in ms.
     */
    static unsigned long long GetNow();

private:
    double m_time = 0.0f;
    float m_deltaTime = 0.0f;

    Time() {}
    static Time* GetInstance();

    friend class Behaviour;
    friend class GameObject;
    friend class Application;
    friend class SceneManager;
};

#endif // TIME_H
