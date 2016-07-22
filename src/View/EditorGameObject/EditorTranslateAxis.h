#ifndef EDITORTRANSLATEAXIS_H
#define EDITORTRANSLATEAXIS_H

#include "Bang.h"

#include "Matrix4.h"
#include "Material.h"
#include "Transform.h"
#include "EditorAxis.h"
#include "EditorGameObject.h"
#include "SingleLineRenderer.h"

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
};

#endif // EDITORTRANSLATEAXIS_H
