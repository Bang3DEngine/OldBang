#ifndef EDITORAXISGROUP_H
#define EDITORAXISGROUP_H

#include "Transform.h"
#include "EditorAxis.h"
#include "MeshRenderer.h"
#include "LineRenderer.h"
#include "EditorGameObject.h"

/**
 * @brief The EditorAxisGroup class represents a group of EditorAxises.
 * There will be X,Y,Z EditorAxis, which will be children of this EditorGameObject.
 * We need to handle them separately to be able to detect the mouse events separately
 * on each one.
 */
class EditorAxisGroup : public EditorGameObject
{
private:
    bool globalCoords = false;

    EditorAxis *axisX;
    EditorAxis *axisY;
    EditorAxis *axisZ;

public:
    EditorAxisGroup();
    virtual ~EditorAxisGroup();

    void OnUpdate() override;

    void OnButtonGlobalCoordsClicked(bool globalCoords) override;
};

#endif // EDITORAXIS_H
