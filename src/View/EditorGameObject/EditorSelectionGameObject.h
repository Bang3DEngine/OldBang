#ifndef EDITORSELECTIONGAMEOBJECT_H
#define EDITORSELECTIONGAMEOBJECT_H

#include "EditorBBox.h"
#include "EditorAxisGroup.h"
#include "EditorGameObject.h"

/**
 * @brief Contains all the GameObjects that are added
 * to the selected GameObject when selecting it.
 * It contains:
 *      - EditorAxis
 *      - EditorBBox
 */
class EditorSelectionGameObject : public EditorGameObject
{
private:
    EditorAxisGroup *axisGroup = nullptr;
    EditorBBox *bbox = nullptr;

public:
    EditorSelectionGameObject();
};

#endif // EDITORSELECTIONGAMEOBJECT_H
