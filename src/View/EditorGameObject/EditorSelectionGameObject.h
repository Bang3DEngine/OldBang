#ifndef EDITORSELECTIONGAMEOBJECT_H
#define EDITORSELECTIONGAMEOBJECT_H

#include "EditorGameObject.h"

class EditorAxisGroup;
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
    EditorAxisGroup *m_axisGroup = nullptr;

public:
    EditorSelectionGameObject(GameObject *attachedGameObject);
    virtual ~EditorSelectionGameObject();
};

#endif // EDITORSELECTIONGAMEOBJECT_H
