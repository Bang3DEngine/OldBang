#ifndef EDITORROTATEAXISGROUP_H
#define EDITORROTATEAXISGROUP_H

#include "Bang/GameObject.h"

class Toolbar;
class MeshRenderer;
class EditorRotateAxis;
class EditorRotateAxisGroup : public GameObject
{
private:
    EditorRotateAxis *m_axisX = nullptr;
    EditorRotateAxis *m_axisY = nullptr;
    EditorRotateAxis *m_axisZ = nullptr;
    CircleRenderer *m_circle = nullptr;

public:
    EditorRotateAxisGroup(GameObject *attachedGameObject);
    virtual ~EditorRotateAxisGroup();

    void OnEditorUpdate() override;
    void OnDrawGizmos(bool depthed, bool overlay) override;
};

#endif // EDITORROTATEAXISGROUP_H
