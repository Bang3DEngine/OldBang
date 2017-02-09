#ifndef UIGAMEOBJECT_H
#define UIGAMEOBJECT_H

#include "GameObject.h"

class XMLNode;
class UIGameObject : public GameObject
{
public:
    UIGameObject(const String &name = "");

    void FillXMLInfo(XMLNode *xmlInfo) const override;

};

#endif // UIGAMEOBJECT_H
