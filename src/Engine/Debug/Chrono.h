#ifndef CHRONO_H
#define CHRONO_H

#include "Bang/Array.h"
#include "Bang/String.h"

class Chrono
{
public:
    Chrono(const String &chronoName = "");

    void MarkEvent(const String &eventName);
    void Log();

private:
    struct ChronoEvent
    {
        String eventName        = "";
        long time               = 0;
        long timeSinceLastEvent = 0;
    };

    String m_chronoName = "";
    Array<ChronoEvent> m_events = {};
};

#endif // CHRONO_H
