#ifndef COMPONENT_H
#define COMPONENT_H

#include "Bang/Object.h"
#include "Bang/IToString.h"
#include "Bang/RenderPass.h"
#include "Bang/Serializable.h"

#define COMPONENT(ClassName) \
    SERIALIZABLE(ClassName) \
    friend class ComponentFactory;

class Component : public Object,
                  public Serializable,
                  public IToString
{
    COMPONENT(Component)

public:
    GameObject* const& gameObject = m_gameObject;

    void SetGameObject(GameObject *gameObject);

    GameObject *GetGameObject() const;

    virtual void OnStart();
    virtual void OnUpdate();
    virtual void OnParentSizeChanged();
    virtual void OnRender(RenderPass renderPass);
    virtual void OnBeforeChildrenRender(RenderPass renderPass);
    virtual void OnChildrenRendered(RenderPass renderPass);
    virtual void OnRenderGizmos();
    virtual void OnDestroy();

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // IToString
    virtual String ToString() const override;

    // Serializable
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
