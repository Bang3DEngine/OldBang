#ifndef SELECTION_H
#define SELECTION_H

#include "Bang.h"

FORWARD class SelectionFramebuffer;

class Selection
{
public:
    static GameObject *GetOveredGameObject();
    Selection() = delete;

private:
    static SelectionFramebuffer* GetSelectionFramebuffer();
};

#endif // SELECTION_H
