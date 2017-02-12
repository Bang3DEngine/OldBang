#ifndef EDITORSELECTIONGAMEOBJECT_H
#define EDITORSELECTIONGAMEOBJECT_H

#include "EditorGameObject.h"

class EditorAxisGroup;
class EditorRectTransformGizmo;
/**
 * @brief Contains all the GameObjects that are added
 * to the selected GameObject when selecting it.
 * It contains:
 *      - EditorTranslateAxis
 */
class EditorSelectionGameObject : public EditorGameObject
{
public:
    EditorSelectionGameObject(GameObject *attachedGameObject);
    virtual ~EditorSelectionGameObject();

private:
    EditorAxisGroup *m_axisGroup = nullptr;
    EditorRectTransformGizmo *m_rectTransformGizmo = nullptr;
};

#endif // EDITORSELECTIONGAMEOBJECT_H
