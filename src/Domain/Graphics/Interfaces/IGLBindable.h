#ifndef IGLBINDABLE_H
#define IGLBINDABLE_H

#include "Bang.h"

#include <GL/glew.h>
#include <iostream>
#include <vector>
#include <stack>

class IGLBindable
{
private:
    void CreateEnoughLatestBoundIdsPositions(unsigned  int index) const;

protected:
    mutable std::vector<GLint> latestBoundId;

    IGLBindable();

    void PreBind(GLenum bindTarget, unsigned int index = 0) const;
    GLint PreUnBind(unsigned int index = 0) const;

    virtual void Bind() const = 0;
    virtual void UnBind() const = 0;
};

#endif // IGLBINDABLE_H
