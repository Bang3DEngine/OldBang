#ifndef SELECTION_H
#define SELECTION_H

#include "Bang.h"

FORWARD class SelectionFramebuffer;

class Selection
{
public:
    static GameObject *GetOveredGameObject();

private:
    static SelectionFramebuffer* GetSelectionFramebuffer();

    Selection() = delete;
};

#endif // SELECTION_H
