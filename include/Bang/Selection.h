#ifndef SELECTION_H
#define SELECTION_H

#include "Bang/Bang.h"

NAMESPACE_BANG_BEGIN

FORWARD class SelectionFramebuffer;

class Selection
{
public:
    static GameObject *GetOveredGameObject();
    Selection() = delete;

private:
    static SelectionFramebuffer* GetSelectionFramebuffer();
};

NAMESPACE_BANG_END

#endif // SELECTION_H
