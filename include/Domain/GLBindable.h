#ifndef GLBINDABLE_H
#define GLBINDABLE_H

#include <GL/glew.h>
#include <iostream>
#include <stack>

class GLBindable
{
protected:
    mutable std::stack<GLint> latestBoundIds;

    GLBindable() {}

    void PreBind(GLenum bindTarget) const
    {
        GLint lastBoundId;
        glGetIntegerv(bindTarget, &lastBoundId);
        latestBoundIds.push(lastBoundId);
    }

    GLint PreUnBind() const
    {
        if(latestBoundIds.empty()) return 0;
        GLint lastBoundId = latestBoundIds.top();
        latestBoundIds.pop();
        return lastBoundId;
    }

    virtual void Bind() const = 0;
    virtual void UnBind() const = 0;
};

#endif // GLBINDABLE_H
