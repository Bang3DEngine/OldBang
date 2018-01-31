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
    friend class Bang::ObjectManager;

class Component : public Object,
                  public IToString
{
    COMPONENT(Component)

public:
    static void Destroy(Component *component);

    void SetGameObject(GameObject *gameObject);

    GameObject *GetGameObject() const;

    bool IsEnabled(bool recursive = false) const;

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // IToString
    virtual String ToString() const override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

protected:
    Component();
    virtual ~Component();

    virtual void OnPreStart() override;
    virtual void OnStart() override;
    virtual void OnPreUpdate();
    virtual void OnBeforeChildrenUpdate();
    virtual void OnUpdate();
    virtual void OnAfterChildrenUpdate();
    virtual void OnPostUpdate();
    virtual void OnBeforeRender();
    virtual void OnBeforeChildrenRender(RenderPass renderPass);
    virtual void OnRender(RenderPass renderPass);
    virtual void OnAfterChildrenRender(RenderPass renderPass);
    virtual void OnDestroy() override;

    void PreUpdate();
    void BeforeChildrenUpdate();
    void Update();
    void AfterChildrenUpdate();
    void PostUpdate();
    void BeforeRender();
    void BeforeChildrenRender(RenderPass renderPass);
    void Render(RenderPass renderPass);
    void AfterChildrenRender(RenderPass renderPass);


    virtual bool CanBeRepeatedInGameObject() const;

private:
    GameObject *p_gameObject = nullptr;

    friend class GameObject;
};

NAMESPACE_BANG_END

#endif // COMPONENT_H
