#ifndef COMPONENT_H
#define COMPONENT_H

#include "Bang/SceneNode.h"
#include "Bang/IToString.h"
#include "Bang/SerializableObject.h"

#define COMPONENT(ClassName) \
    SOBJECT(ClassName) \
    friend class ComponentFactory;

class Component : public SceneAgent,
                  public IToString,
                  public SerializableObject
{
    COMPONENT(Component)

public:
    GameObject* const& gameObject = m_gameObject;
    Transform* const& transform = m_gameObjectTransform;

    virtual void CloneInto(ICloneable *clone) const override;

    virtual String ToString() const override;

    void AddDelegate(Component *delegate);
    void RemoveDelegate(Component *delegate);
    const List<Component*>& GetDelegates() const;

    GameObject *GetGameObject() const;
    void SetGameObject(GameObject *gameObject);

    virtual String GetInstanceId() const override;
    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

    virtual void Render(RenderPass renderPass);

protected:
    Component();
    virtual ~Component();

    virtual void Start();
    virtual void Update();
    virtual void ParentSizeChanged();
    virtual void RenderGizmos();
    virtual void Destroy();

private:
    List<Component*> m_delegates;
    GameObject *m_gameObject = nullptr;
    Transform *m_gameObjectTransform = nullptr;

    friend class GameObject;
};

#endif // COMPONENT_H
