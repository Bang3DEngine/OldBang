#ifndef EDITORROTATEAXISGROUP_H
#define EDITORROTATEAXISGROUP_H

#include "EditorGameObject.h"
#include "EditorRotateAxis.h"

class Toolbar;
class MeshRenderer;
class EditorRotateAxisGroup : public EditorGameObject
{
private:
    EditorRotateAxis *m_axisX = nullptr;
    EditorRotateAxis *m_axisY = nullptr;
    EditorRotateAxis *m_axisZ = nullptr;

public:
    EditorRotateAxisGroup(GameObject *attachedGameObject);
    virtual ~EditorRotateAxisGroup();

    void OnUpdate() override;
    void OnDrawGizmosNoDepth() override;
};

#endif // EDITORROTATEAXISGROUP_H
