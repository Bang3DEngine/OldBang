#ifndef UILAYOUTMANAGER_H
#define UILAYOUTMANAGER_H

#include "Bang/Bang.h"

NAMESPACE_BANG_BEGIN

FORWARD class Scene;

class UILayoutManager
{
public:
    UILayoutManager() = delete;

    static void RebuildLayout(Scene *scene, bool force = false);
};

NAMESPACE_BANG_END

#endif // UILAYOUTMANAGER_H
