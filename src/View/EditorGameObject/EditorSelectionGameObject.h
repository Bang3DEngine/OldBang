#ifndef EDITORSELECTIONGAMEOBJECT_H
#define EDITORSELECTIONGAMEOBJECT_H

#include "EditorBBox.h"
#include "EditorTranslateAxisGroup.h"
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
    EditorTranslateAxisGroup *axisGroup = nullptr;
    EditorBBox *bbox = nullptr;

public:
    EditorSelectionGameObject();
};

#endif // EDITORSELECTIONGAMEOBJECT_H
