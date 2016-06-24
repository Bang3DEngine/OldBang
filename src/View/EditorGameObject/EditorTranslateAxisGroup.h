#ifndef EDITORTRANSLATEAXISGROUP_H
#define EDITORTRANSLATEAXISGROUP_H

#include "Toolbar.h"
#include "Transform.h"
#include "EditorAxis.h"
#include "MeshRenderer.h"
#include "LineRenderer.h"
#include "EditorGameObject.h"
#include "EditorTranslateAxis.h"

/**
 * @brief The EditorTranslateAxisGroup class represents a group of EditorTranslateAxises.
 * There will be X,Y,Z EditorTranslateAxis, which will be children of this EditorGameObject.
 * We need to handle them separately to be able to detect the mouse events separately
 * on each one.
 */
class EditorTranslateAxisGroup : public EditorGameObject
{
private:
    EditorTranslateAxis *axisX;
    EditorTranslateAxis *axisY;
    EditorTranslateAxis *axisZ;

public:
    EditorTranslateAxisGroup();
    virtual ~EditorTranslateAxisGroup();

    void OnUpdate() override;
};

#endif // EDITORTRANSLATEAXIS_H
