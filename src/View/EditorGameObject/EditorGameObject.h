#ifndef EDITORGAMEOBJECT_H
#define EDITORGAMEOBJECT_H

#include "Bang.h"

#include "GameObject.h"

class EditorGameObject : public GameObject
{
public:
    EditorGameObject();
    virtual ~EditorGameObject();

    #ifdef BANG_EDITOR
    //They are empty, since you can't and shouldn't save or load
    //editor internal instances.
    void Write(std::ostream &f) const override;
    void Read(std::istream &f) override;
    #endif

};

#endif // EDITORGAMEOBJECT_H
