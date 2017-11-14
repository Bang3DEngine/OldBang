#ifndef COMPONENT_H
#define COMPONENT_H

#include "Bang/Object.h"
#include "Bang/IToString.h"
#include "Bang/RenderPass.h"
#include "Bang/Serializable.h"
#include "Bang/IEventEmitter.h"
#include "Bang/ObjectManager.h"
#include "Bang/ComponentFactory.h"
#include "Bang/IDestroyListener.h"

NAMESPACE_BANG_BEGIN

#define REQUIRE_COMPONENT(gameObject, ComponentClass) \
    ASSERT(gameObject->HasComponent<ComponentClass>())

#define COMPONENT(ClassName) \
    public: virtual ClassName *Clone() const override {\
        ClassName *clone = ComponentFactory::Create<ClassName>();\
        CloneInto(clone);\
        return clone;\
    }\
    SERIALIZABLE(ClassName) \
    friend class ObjectManager;

class Component : public Object,
                  public Serializable,
                  public IToString
{
    COMPONENT(Component)

public:
    void SetGameObject(GameObject *gameObject);

    GameObject *GetGameObject() const;

    bool IsEnabled(bool recursive = false) const;

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

    virtual void OnStart() override;
    virtual void OnPreUpdate();
    virtual void OnUpdate();
    virtual void OnPostUpdate();
    virtual void OnBeforeChildrenRender(RenderPass renderPass);
    virtual void OnRender(RenderPass renderPass);
    virtual void OnRenderGizmos();
    virtual void OnAfterChildrenRender(RenderPass renderPass);
    virtual void OnDestroy() override;

private:
    GameObject *p_gameObject = nullptr;

    friend class GameObject;
};

NAMESPACE_BANG_END

#endif // COMPONENT_H
