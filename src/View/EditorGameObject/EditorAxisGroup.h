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
    const float c_sizeBoost = 0.2f;

    GameObject *p_attachedGameObject = nullptr;

    EditorTranslateAxisGroup *p_etag = nullptr;
    EditorRotateAxisGroup *p_erag = nullptr;
    EditorScaleAxisGroup *p_esag = nullptr;

    void DisableAllGroups();

public:
    EditorAxisGroup(GameObject *p_attachedGameObject);

    virtual void OnUpdate() override;
};

#endif // EDITORAXISGROUP_H
