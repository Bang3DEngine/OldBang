#ifndef EDITORSCALEAXISGROUP_H
#define EDITORSCALEAXISGROUP_H

#include "Toolbar.h"
#include "Transform.h"
#include "EditorAxis.h"
#include "MeshRenderer.h"
#include "LineRenderer.h"
#include "EditorGameObject.h"
#include "EditorScaleAxis.h"

/**
 * @brief The EditorScaleAxisGroup class represents a group of EditorScaleAxises.
 * There will be X,Y,Z EditorScaleAxis, which will be children of this EditorGameObject.
 * We need to handle them separately to be able to detect the mouse events separately
 * on each one.
 */
class EditorScaleAxisGroup : public EditorGameObject
{
private:
    EditorScaleAxis *m_axisX;
    EditorScaleAxis *m_axisY;
    EditorScaleAxis *m_axisZ;

public:
    EditorScaleAxisGroup(GameObject *attachedGameObject);
    virtual ~EditorScaleAxisGroup();

    void OnUpdate() override;
};

#endif // EDITORSCALEAXIS_H
