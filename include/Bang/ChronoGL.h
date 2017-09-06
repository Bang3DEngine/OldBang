#ifndef CHRONOGL_H
#define CHRONOGL_H

#include <GL/glew.h>

#include "Bang/Array.h"
#include "Bang/String.h"

NAMESPACE_BANG_BEGIN

class ChronoGL
{
public:
    ChronoGL(const String &chronoName = "");
    virtual ~ChronoGL();

    void MarkEvent(const String &eventName);
    void Log();

private:
    struct ChronoGLEvent
    {
        String eventName        = "";
        GLuint queryId          = 0;
        long timeSinceLastEvent = 0;
    };

    String m_chronoName = "";
    Array<ChronoGLEvent> m_events = {};
};

NAMESPACE_BANG_END

#endif // CHRONOGL_H
