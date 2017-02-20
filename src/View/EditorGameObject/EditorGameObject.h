#ifndef EDITORGAMEOBJECT_H
#define EDITORGAMEOBJECT_H

#include "GameObject.h"

class EditorGameObject : public GameObject
{
public:
    EditorGameObject(const String &name = "EditorGameObject");
    virtual ~EditorGameObject();
};

#endif // EDITORGAMEOBJECT_H
