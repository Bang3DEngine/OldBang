#ifndef CHRONO_H
#define CHRONO_H

#include <vector>
#include <iostream>

#include "String.h"

class Chrono
{
private:
    struct ChronoEvent
    {
        String eventName;
        long time;
        long timeSinceLastEvent;
    };

    String m_chronoName;
    std::vector<ChronoEvent> m_events;

public:
    Chrono(String chronoName = "");
    void MarkEvent(const String &eventName);
    void Log();
};

#endif // CHRONO_H
