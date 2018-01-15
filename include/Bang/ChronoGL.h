#ifndef CHRONOGL_H
#define CHRONOGL_H

#include <GL/glew.h>

#include "Bang/Array.h"
#include "Bang/String.h"

NAMESPACE_BANG_BEGIN

class ChronoGL
{
public:
    ChronoGL();
    virtual ~ChronoGL();

    void MarkBegin();
    void MarkEnd();

    double GetEllapsedSeconds() const;

private:
    GLuint m_queryId = 0;
    mutable GLuint m_prevTimeNanos = 0;

    bool IsQueryResultAvailable() const;
    GLuint GetQueryResultNanos() const;
};

NAMESPACE_BANG_END

#endif // CHRONOGL_H
