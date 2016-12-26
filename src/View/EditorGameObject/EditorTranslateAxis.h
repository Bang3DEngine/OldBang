#ifndef EDITORTRANSLATEAXIS_H
#define EDITORTRANSLATEAXIS_H

#include "EditorAxis.h"

class EditorGameObject;
class SingleLineRenderer;
class EditorTranslateAxis : public EditorAxis
{
private:
    EditorGameObject *m_axisCap = nullptr;
    SingleLineRenderer *m_line = nullptr;

protected:

    virtual Renderer *GetAxisRenderer() const override;

public:
    EditorTranslateAxis(EditorAxis::EditorAxisDirection dir, GameObject *m_attachedGameObject);
    virtual ~EditorTranslateAxis();

    void OnUpdate() override;
    void OnDrawGizmosOverlay() override;
};

#endif // EDITORTRANSLATEAXIS_H
