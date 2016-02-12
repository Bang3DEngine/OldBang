#include "GLBindable.h"

GLBindable::GLBindable() {}

void GLBindable::CreateEnoughLatestBoundIdsPositions(unsigned int index) const
{
    while(latestBoundId.size() <= index)
        latestBoundId.push_back(0);
}

void GLBindable::PreBind(GLenum bindTarget, unsigned int index) const
{
    CreateEnoughLatestBoundIdsPositions(index);
    glGetIntegerv(bindTarget, &latestBoundId[index]);
}

GLint GLBindable::PreUnBind(unsigned int index) const
{
    return latestBoundId[index];
}
