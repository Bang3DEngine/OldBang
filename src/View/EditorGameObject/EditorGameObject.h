#ifndef EDITORGAMEOBJECT_H
#define EDITORGAMEOBJECT_H

#include "GameObject.h"

class EditorGameObject : public GameObject
{
public:
    EditorGameObject();
    EditorGameObject(const String &name);
    virtual ~EditorGameObject();

    virtual void CloneInto(ICloneable *clone) const override;
    virtual ICloneable* Clone() const override;

    virtual bool IsEditorGameObject() const override;

    bool IsRenderInSelectionFramebuffer() const;

    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;

protected:
    bool m_renderInSelectionFramebuffer = true;
};

#endif // EDITORGAMEOBJECT_H
