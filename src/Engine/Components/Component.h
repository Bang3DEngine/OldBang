#ifndef COMPONENT_H
#define COMPONENT_H

#include "Bang/Object.h"
#include "Bang/IToString.h"
#include "Bang/SerializableObject.h"
#include "Bang/ISceneEventListener.h"

FORWARD class String;
FORWARD class XMLNode;
FORWARD class Transform;
FORWARD class GameObject;

class Component : public ISceneEventListener,
                  public IToString,
                  public SerializableObject
{
    OBJECT(Component)

public:
    GameObject *m_gameObject = nullptr;
    Transform *m_gameObjectTransform = nullptr;
    GameObject* const& gameObject = m_gameObject;
    Transform* const& transform = m_gameObjectTransform; // shortcut

    virtual void CloneInto(ICloneable *clone) const override;

    virtual String ToString() const override;

    void SetGameObject(GameObject *gameObject);

    void SetEnabled(bool enabled);
    bool IsEnabled(bool recursive = true) const;

    virtual String GetInstanceId() const override;
    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

protected:
    bool m_enabled = true;

    Component();
    virtual ~Component();
};

#endif // COMPONENT_H
