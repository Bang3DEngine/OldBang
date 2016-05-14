#ifndef EDITORSELECTIONGAMEOBJECT_H
#define EDITORSELECTIONGAMEOBJECT_H

#include "EditorAxis.h"
#include "EditorBBox.h"
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
    EditorAxis *axis = nullptr;
    EditorBBox *bbox = nullptr;

public:
    EditorSelectionGameObject();
};

#endif // EDITORSELECTIONGAMEOBJECT_H
