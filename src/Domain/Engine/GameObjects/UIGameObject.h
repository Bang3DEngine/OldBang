#ifndef UIGAMEOBJECT_H
#define UIGAMEOBJECT_H

#include "List.h"
#include "GameObject.h"

class XMLNode;
class ICloneable;
class UIGameObject : public virtual GameObject
{
public:
    UIGameObject(const String &name = "");

    virtual void CloneInto(ICloneable *clone) const override;
    virtual ICloneable *Clone() const override;

    void FillXMLInfo(XMLNode *xmlInfo) const override;

    #ifdef BANG_EDITOR
    void OnHierarchyGameObjectsSelected(
            List<GameObject*> &selectedGameObjects) override;
    #endif
};

#endif // UIGAMEOBJECT_H
