#ifndef EDITORTRANSLATEAXIS_H
#define EDITORTRANSLATEAXIS_H

#include "Bang/EditorAxis.h"

class GameObject;
class SingleLineRenderer;
class EditorTranslateAxis : public EditorAxis
{
private:
    GameObject *m_axisCap = nullptr;
    SingleLineRenderer *m_line = nullptr;

protected:

    virtual Renderer *GetAxisRenderer() const override;

public:
    EditorTranslateAxis(EditorAxis::EditorAxisDirection dir, GameObject *m_attachedGameObject);
    virtual ~EditorTranslateAxis();

    void OnEditorUpdate() override;
    void OnDrawGizmos(bool depthed, bool overlay) override;
};

#endif // EDITORTRANSLATEAXIS_H
