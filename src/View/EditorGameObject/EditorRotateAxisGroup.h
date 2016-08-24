#ifndef EDITORROTATEAXISGROUP_H
#define EDITORROTATEAXISGROUP_H

#include "Toolbar.h"
#include "EditorGameObject.h"
#include "EditorRotateAxis.h"

class EditorRotateAxisGroup : public EditorGameObject
{
private:
    EditorRotateAxis *m_axisX = nullptr;
    EditorRotateAxis *m_axisY = nullptr;
    EditorRotateAxis *m_axisZ = nullptr;

public:
    EditorRotateAxisGroup(GameObject *attachedGameObject);

    void OnUpdate() override;
    void OnDrawGizmosNoDepth() override;
};

#endif // EDITORROTATEAXISGROUP_H
