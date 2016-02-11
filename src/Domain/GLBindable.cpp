#include "GLBindable.h"

GLBindable::GLBindable() {}

void GLBindable::CreateEnoughLatestBoundIdsStacks(unsigned int latestBindStackId) const
{
    while(latestBoundIds.size() <= latestBindStackId)
        latestBoundIds.push_back(std::stack<GLint>());
}

void GLBindable::PreBind(GLenum bindTarget, unsigned int latestBindStackId) const
{
    CreateEnoughLatestBoundIdsStacks(latestBindStackId);

    GLint lastBoundId;
    glGetIntegerv(bindTarget, &lastBoundId);
    latestBoundIds[latestBindStackId].push(lastBoundId);
}

GLint GLBindable::PreUnBind(unsigned int latestBindStackId) const
{
    CreateEnoughLatestBoundIdsStacks(latestBindStackId);

    if(latestBoundIds[latestBindStackId].empty()) return 0;
    GLint lastBoundId = latestBoundIds[latestBindStackId].top();
    latestBoundIds[latestBindStackId].pop();
    return lastBoundId;
}
