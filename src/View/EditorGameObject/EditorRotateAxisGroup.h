#ifndef EDITORROTATEAXISGROUP_H
#define EDITORROTATEAXISGROUP_H

#include "Toolbar.h"
#include "EditorGameObject.h"
#include "EditorRotateAxis.h"

class EditorRotateAxisGroup : public EditorGameObject
{
private:
    EditorRotateAxis *p_axisX = nullptr;
    EditorRotateAxis *p_axisY = nullptr;
    EditorRotateAxis *p_axisZ = nullptr;

public:
    EditorRotateAxisGroup(GameObject *attachedGameObject);

    void OnUpdate() override;
};

#endif // EDITORROTATEAXISGROUP_H
