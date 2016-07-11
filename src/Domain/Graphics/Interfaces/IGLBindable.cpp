#include "IGLBindable.h"
#include "Logger.h"

IGLBindable::IGLBindable() {}

void IGLBindable::PreBind(GLenum bindTarget) const
{
    if(m_boundIds.find(bindTarget) == m_boundIds.end())
    {
        m_boundIds[bindTarget] = std::stack<GLint>();
    }

    GLint currentBoundId = 0;
    glGetIntegerv(bindTarget, &currentBoundId);
    m_boundIds[bindTarget].push(currentBoundId);
}

GLint IGLBindable::PreUnBind(GLenum bindTarget) const
{
    if(m_boundIds.find(bindTarget) == m_boundIds.end() ||
       m_boundIds[bindTarget].empty())
    {
        Logger_Warn("Unbinding non-binded GL target.");
        return 0;
    }

    GLint id = m_boundIds[bindTarget].top();
    m_boundIds[bindTarget].pop();
    return id;
}
