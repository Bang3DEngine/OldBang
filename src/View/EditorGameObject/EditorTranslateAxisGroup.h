#ifndef EDITORTRANSLATEAXISGROUP_H
#define EDITORTRANSLATEAXISGROUP_H

#include "EditorGameObject.h"

class EditorAxisPlane;
class EditorTranslateAxis;
/**
 * @brief The EditorTranslateAxisGroup class represents a group of EditorTranslateAxises.
 * There will be X,Y,Z EditorTranslateAxis, which will be children of this EditorGameObject.
 * We need to handle them separately to be able to detect the mouse events separately
 * on each one.
 */
class EditorTranslateAxisGroup : public EditorGameObject
{
private:
    EditorTranslateAxis *m_axisX = nullptr;
    EditorTranslateAxis *m_axisY = nullptr;
    EditorTranslateAxis *m_axisZ = nullptr;
    EditorAxisPlane *m_planeXY = nullptr;
    EditorAxisPlane *m_planeXZ = nullptr;
    EditorAxisPlane *m_planeYZ = nullptr;

public:
    EditorTranslateAxisGroup(GameObject *attachedGameObject);
    virtual ~EditorTranslateAxisGroup();

    void OnUpdate() override;
};

#endif // EDITORTRANSLATEAXIS_H
