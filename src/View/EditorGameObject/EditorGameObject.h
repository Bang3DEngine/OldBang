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

    /**
     * @brief Returns the GameObject attached to this axis.
     * This is, the GameObject this axis is referring/bound to.
     * To be specific, returns the first object in the parent hierarchy which
     * is not an EditorGameObject.
     * @return The attached GameObject to this axis
     */
    virtual GameObject *GetAttachedGameObject() const;

    void Write(std::ostream &f) const override;
};

#endif // EDITORGAMEOBJECT_H
