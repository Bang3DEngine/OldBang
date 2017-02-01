#include "Chrono.h"

#include <iostream>

#include "Time.h"


Chrono::Chrono(String chronoName)
{
    m_chronoName = chronoName;
}

void Chrono::MarkEvent(const String &eventName)
{
    ChronoEvent ce;
    ce.eventName = eventName;
    ce.time = Time::GetNow();
    if (m_events.Empty())
    {
        ce.timeSinceLastEvent = 0;
    }
    else
    {
        ChronoEvent &previousEvent = m_events.Back();

        // Update previousEvent
        previousEvent.timeSinceLastEvent = ce.time - previousEvent.time;
    }

    m_events.PushBack(ce);
}

void Chrono::Log()
{
    if (m_events.Empty()) return;

    std::cerr << "Chrono " <<  m_chronoName << " -------------------" << std::endl;
    long now = Time::GetNow();
    for (int i = 0; i < m_events.Size(); ++i)
    {
        ChronoEvent &ce = m_events[i];
        float intervalSecs = 0.0f;
        if (i < m_events.Size() - 1)
        {
            intervalSecs = ce.timeSinceLastEvent / 1000.0f;
        }
        else
        {
            intervalSecs = (now - ce.time) / 1000.0f;
        }

        std::cerr << ce.eventName << "(" <<  intervalSecs << ")[" << ce.time << "]" << std::endl;
    }
    long firstTime = m_events.Front().time;
    std::cerr << "Total: " << (now - firstTime) / 1000.0f << " ----------------" << std::endl;
    std::cerr << std::endl;
}
