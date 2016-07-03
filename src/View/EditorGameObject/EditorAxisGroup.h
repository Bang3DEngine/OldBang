#ifndef EDITORAXISGROUP_H
#define EDITORAXISGROUP_H

#include "Toolbar.h"
#include "EditorScaleAxisGroup.h"
#include "EditorRotateAxisGroup.h"
#include "EditorTranslateAxisGroup.h"

/**
 * @brief The EditorAxisGroup class contains the Translate, Rotate
 * and Scale Axis Groups.
 *
 * This is the responsible of enabling or disabling them, depending on
 * the mode selected in the Toolbar
 */
class EditorAxisGroup : public EditorGameObject
{

private:
    float sizeBoost = 0.2f;

    GameObject *attachedGameObject = nullptr;

    EditorTranslateAxisGroup *etag = nullptr;
    EditorRotateAxisGroup *erag = nullptr;
    EditorScaleAxisGroup *esag = nullptr;

    void DisableAllGroups();

public:
    EditorAxisGroup(GameObject *attachedGameObject);

    virtual void OnUpdate() override;
};

#endif // EDITORAXISGROUP_H
