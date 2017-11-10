#ifndef TIMER_H
#define TIMER_H

#include "Bang/List.h"
#include "Bang/Component.h"

NAMESPACE_BANG_BEGIN

class Timer : public Component
{
    COMPONENT(Timer)

public:
    void Run();
    void Stop();

    // Component
    void OnUpdate() override;

    void AddCallback(std::function<void()> callback);
    void SetInterval(float intervalSecs);

    bool IsRunning() const;
    float GetInterval() const;

protected:
	Timer();
	virtual ~Timer();

private:
    bool m_running = true;
    float m_intervalSecs = 1.0f;
    uint64_t m_lastTickTimestampSecs = 0;

    List<std::function<void()>> m_callbacksList;

    void Tick();
    void ResetTimestamp();
};

NAMESPACE_BANG_END

#endif // TIMER_H

