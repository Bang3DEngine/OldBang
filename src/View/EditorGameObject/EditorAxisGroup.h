#ifndef EDITORAXISGROUP_H
#define EDITORAXISGROUP_H

#include "GameObject.h"

class GameObject;
class EditorScaleAxisGroup;
class EditorRotateAxisGroup;
class EditorTranslateAxisGroup;
/**
 * @brief The EditorAxisGroup class contains the Translate, Rotate
 * and Scale Axis Groups.
 *
 * This is the responsible of enabling or disabling them, depending on
 * the mode selected in the Toolbar
 */
class EditorAxisGroup : public GameObject
{

private:
    const float c_sizeBoost = 0.2f;

    GameObject *m_attachedGameObject = nullptr;

    EditorTranslateAxisGroup *m_etag = nullptr;
    EditorRotateAxisGroup *m_erag = nullptr;
    EditorScaleAxisGroup *m_esag = nullptr;

    void DisableAllGroups();

public:
    EditorAxisGroup(GameObject *m_attachedGameObject);

    virtual void OnUpdate() override;
};

#endif // EDITORAXISGROUP_H
