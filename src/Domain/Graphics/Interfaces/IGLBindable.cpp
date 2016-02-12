#include "IGLBindable.h"

IGLBindable::IGLBindable() {}

void IGLBindable::CreateEnoughLatestBoundIdsPositions(unsigned int index) const
{
    while(latestBoundId.size() <= index)
        latestBoundId.push_back(0);
}

void IGLBindable::PreBind(GLenum bindTarget, unsigned int index) const
{
    CreateEnoughLatestBoundIdsPositions(index);
    glGetIntegerv(bindTarget, &latestBoundId[index]);
}

GLint IGLBindable::PreUnBind(unsigned int index) const
{
    return latestBoundId[index];
}
