#ifndef EDITORGAMEOBJECT_H
#define EDITORGAMEOBJECT_H

#include "GameObject.h"

class EditorGameObject : public GameObject
{
protected:
    bool m_renderInSelectionFramebuffer = true;

public:
    EditorGameObject();
    EditorGameObject(const String &name);

    virtual void CloneInto(ICloneable *clone) const override;
    virtual ICloneable* Clone() const;

    virtual bool IsEditorGameObject() const override;

    bool IsRenderInSelectionFramebuffer() const;

    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;
};

#endif // EDITORGAMEOBJECT_H
