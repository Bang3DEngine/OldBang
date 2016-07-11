#ifndef EDITORGAMEOBJECT_H
#define EDITORGAMEOBJECT_H

#include "GameObject.h"

class EditorGameObject : public GameObject
{
public:
    EditorGameObject();
    EditorGameObject(const std::string &m_name);

    virtual void CloneInto(ICloneable *clone) const override;
    virtual ICloneable* Clone() const;

    virtual bool IsEditorGameObject() const override;

    void Write(std::ostream &f) const override;
};

#endif // EDITORGAMEOBJECT_H
