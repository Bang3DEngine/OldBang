#ifndef EDITORSELECTIONGAMEOBJECT_H
#define EDITORSELECTIONGAMEOBJECT_H

#include "EditorBBox.h"
#include "EditorAxisGroup.h"
#include "EditorGameObject.h"

/**
 * @brief Contains all the GameObjects that are added
 * to the selected GameObject when selecting it.
 * It contains:
 *      - EditorTranslateAxis
 *      - EditorBBox
 */
class EditorSelectionGameObject : public EditorGameObject
{
private:
    EditorAxisGroup *p_axisGroup = nullptr;

public:
    EditorSelectionGameObject(GameObject *attachedGameObject);
};

#endif // EDITORSELECTIONGAMEOBJECT_H
