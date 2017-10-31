#include "Bang/ChronoGL.h"

#include <iostream>

#include "Bang/Time.h"

USING_NAMESPACE_BANG

ChronoGL::ChronoGL(const String &chronoName)
{
    m_chronoName = chronoName;
    glFinish();
}

ChronoGL::~ChronoGL()
{
    for (const ChronoGLEvent &ev : m_events)
    {
        glDeleteQueries(1, &ev.queryId);
    }
}

void ChronoGL::MarkEvent(const String &eventName)
{
    ChronoGLEvent cge;
    cge.eventName = eventName;
    if (!m_events.IsEmpty()) // Wait for previous event complete and update it
    {
        glEndQuery(GL_TIME_ELAPSED); // End previous query

        ChronoGLEvent &previousEvent = m_events.Back();

        GLint done = false;
        while (!done)
        {
            glGetQueryObjectiv(previousEvent.queryId,
                               GL_QUERY_RESULT_AVAILABLE, &done);
        }
        GLuint64 previousEventTime;
        glGetQueryObjectui64v(previousEvent.queryId,
                              GL_QUERY_RESULT, &previousEventTime);
        previousEvent.timeSinceLastEvent = previousEventTime;
    }

    glGenQueries(1, &(cge.queryId));
    m_events.PushBack(cge);
    glBeginQuery(GL_TIME_ELAPSED, cge.queryId);
}

void ChronoGL::Log()
{
    if (m_events.IsEmpty()) { return; }

    MarkEvent("EmptyEvent"); // To get the last timer time

    std::cerr << "ChronoGL " <<  m_chronoName
              << " -------------------" << std::endl;
    long totalTime = 0;
    for (int i = 0; i < m_events.Size() - 1; ++i)
    {
        ChronoGLEvent &cge = m_events[i];
        double intervalSecs = cge.timeSinceLastEvent / 1000000000.0;
        totalTime += cge.timeSinceLastEvent;
        std::cerr << "  " <<
                     cge.eventName << ": " <<  intervalSecs << " s." << std::endl;
    }
    std::cerr << "  Total: " << totalTime / 1000000000.0 <<
                 " s. ----------------" << std::endl << std::endl;
}
