#ifndef GLBINDABLE_H
#define GLBINDABLE_H

#include <GL/glew.h>
#include <iostream>
#include <vector>
#include <stack>

class GLBindable
{
private:
    void CreateEnoughLatestBoundIdsPositions(unsigned  int index) const;

protected:
    mutable std::vector<GLint> latestBoundId;

    GLBindable();

    void PreBind(GLenum bindTarget, unsigned int index = 0) const;
    GLint PreUnBind(unsigned int index = 0) const;

    virtual void Bind() const = 0;
    virtual void UnBind() const = 0;
};

#endif // GLBINDABLE_H
