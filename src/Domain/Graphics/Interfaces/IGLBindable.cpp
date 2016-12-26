#include "IGLBindable.h"

#include <iostream>
#include "Debug.h"

IGLBindable::IGLBindable() {}

void IGLBindable::PreBind(GLenum bindTarget) const
{
    if (!m_target_To_BoundIdsStack.ContainsKey(bindTarget))
    {
        m_target_To_BoundIdsStack[bindTarget] = std::stack<GLint>();
    }

    GLint currentBoundId = 0;
    glGetIntegerv(bindTarget, &currentBoundId);
    m_target_To_BoundIdsStack[bindTarget].push(currentBoundId);
}

GLint IGLBindable::PreUnBind(GLenum bindTarget) const
{
    if (!m_target_To_BoundIdsStack.ContainsKey(bindTarget) ||
         m_target_To_BoundIdsStack[bindTarget].empty())
    {
        Debug_Warn("Unbinding non-binded GL target " << bindTarget);
        return 0;
    }

    GLint id = m_target_To_BoundIdsStack[bindTarget].top();
    m_target_To_BoundIdsStack[bindTarget].pop();
    return id;
}
