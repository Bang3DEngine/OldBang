#include "IGLBindable.h"
#include "Logger.h"

IGLBindable::IGLBindable() {}

void IGLBindable::PreBind(GLenum bindTarget) const
{
    if(boundIds.find(bindTarget) == boundIds.end())
    {
        boundIds[bindTarget] = std::stack<GLint>();
    }

    GLint currentBoundId = 0;
    glGetIntegerv(bindTarget, &currentBoundId);
    boundIds[bindTarget].push(currentBoundId);
}

GLint IGLBindable::PreUnBind(GLenum bindTarget) const
{
    if(boundIds.find(bindTarget) == boundIds.end() ||
       boundIds[bindTarget].empty())
    {
        Logger_Warn("Unbinding non-binded GL target.");
        return 0;
    }

    GLint id = boundIds[bindTarget].top();
    boundIds[bindTarget].pop();
    return id;
}
