#ifndef UIGAMEOBJECT_H
#define UIGAMEOBJECT_H

#include "List.h"
#include "GameObject.h"

class XMLNode;
class ICloneable;
class UIGameObject : public GameObject
{
public:
    UIGameObject(const String &name = "");

    virtual void CloneInto(ICloneable *clone) const override;
    virtual ICloneable *Clone() const override;

    void FillXMLInfo(XMLNode *xmlInfo) const override;

    void OnHierarchyGameObjectsSelected(
            List<GameObject*> &selectedGameObjects) override;
};

#endif // UIGAMEOBJECT_H
