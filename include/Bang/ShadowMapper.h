#ifndef SHADOWMAPPER_H
#define SHADOWMAPPER_H

#include "Bang/Bang.h"

NAMESPACE_BANG_BEGIN

FORWARD class Scene;

class ShadowMapper
{
public:
    static AABox GetSceneCastersAABox(Scene *scene);

    ShadowMapper() = delete;
    ~ShadowMapper() = delete;
};

NAMESPACE_BANG_END

#endif // SHADOWMAPPER_H

