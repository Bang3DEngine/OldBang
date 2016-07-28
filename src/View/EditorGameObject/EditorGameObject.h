#ifndef EDITORGAMEOBJECT_H
#define EDITORGAMEOBJECT_H

#include "GameObject.h"

class EditorGameObject : public GameObject
{
public:
    EditorGameObject();
    EditorGameObject(const std::string &name);

    virtual void CloneInto(ICloneable *clone) const override;
    virtual ICloneable* Clone() const;

    virtual bool IsEditorGameObject() const override;

    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;
};

#endif // EDITORGAMEOBJECT_H
