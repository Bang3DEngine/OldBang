#ifndef COMPONENT_H
#define COMPONENT_H

#include "Bang/SceneNode.h"
#include "Bang/IToString.h"
#include "Bang/SerializableObject.h"

#define COMPONENT(ClassName) \
    SERIALIZABLE_OBJECT(ClassName) \
    friend class ComponentFactory;

class Component : public SceneAgent,
                  public IToString,
                  public SerializableObject
{
    COMPONENT(Component)

public:
    GameObject* const& gameObject = m_gameObject;

    void SetGameObject(GameObject *gameObject);

    GameObject *GetGameObject() const;

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // IToString
    virtual String ToString() const override;

    // SerializableObject
    virtual String GetInstanceId() const override;
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

protected:
    Component();
    virtual ~Component();

private:
    GameObject *m_gameObject = nullptr;

    friend class GameObject;
};

#endif // COMPONENT_H
