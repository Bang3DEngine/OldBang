#include "Bang/Chrono.h"

#include <iostream>

#include "Bang/Time.h"

USING_NAMESPACE_BANG

Chrono::Chrono(const String &chronoName)
{
    m_chronoName = chronoName;
}

void Chrono::MarkEvent(const String &eventName)
{
    ChronoEvent ce;
    ce.eventName = eventName;
    ce.time = Time::GetNow();
    if (!m_events.IsEmpty())
    {
        ChronoEvent &previousEvent = m_events.Back();
        previousEvent.timeSinceLastEvent = Time::GetNow() - previousEvent.time;
    }
    m_events.PushBack(ce);
}

void Chrono::Log()
{
    if (m_events.IsEmpty()) { return; }

    MarkEvent("EmptyEvent"); // To get the last timer time

    ::std::cerr << "Chrono " <<  m_chronoName
              << " -------------------" << ::std::endl;
    long totalTime = 0;
    for (int i = 0; i < m_events.Size() - 1; ++i)
    {
        ChronoEvent &ce = m_events[i];
        double intervalSecs = ce.timeSinceLastEvent / 1000.0;
        totalTime += ce.timeSinceLastEvent;
        ::std::cerr << "  " <<
                     ce.eventName << ": " <<  intervalSecs << " s." << ::std::endl;
    }
    ::std::cerr << "  Total: " << totalTime / 1000.0 <<
                 " s.  ----------------" << ::std::endl << ::std::endl;
}
