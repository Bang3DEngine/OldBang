#ifndef EDITORROTATEAXISGROUP_H
#define EDITORROTATEAXISGROUP_H

#include "Toolbar.h"
#include "EditorGameObject.h"
#include "EditorRotateAxis.h"

class EditorRotateAxisGroup : public EditorGameObject
{
private:
    EditorRotateAxis *axisX = nullptr;
    EditorRotateAxis *axisY = nullptr;
    EditorRotateAxis *axisZ = nullptr;

public:
    EditorRotateAxisGroup(GameObject *attachedGameObject);

    void OnUpdate() override;
};

#endif // EDITORROTATEAXISGROUP_H
