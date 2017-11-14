#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <queue>

#include "Bang/List.h"
#include "Bang/Object.h"
#include "Bang/IToString.h"
#include "Bang/RenderPass.h"
#include "Bang/Serializable.h"
#include "Bang/IEventEmitter.h"
#include "Bang/ObjectManager.h"
#include "Bang/IDestroyListener.h"
#include "Bang/IChildrenListener.h"
#include "Bang/GameObjectFactory.h"

NAMESPACE_BANG_BEGIN

FORWARD class Camera;
FORWARD class Component;

#define GAMEOBJECT(ClassName) \
    public: virtual ClassName* Clone() const override {\
        ClassName *clone = GameObject::Create<ClassName>();\
        CloneInto(clone);\
        return clone;\
    }\
    SERIALIZABLE(ClassName) \
    friend class ObjectManager;

class GameObject : public Object,
                   public Serializable,
                   public IToString,
                   public IChildrenListener,
                   public EventEmitter<IChildrenListener>
{
    GAMEOBJECT(GameObject)

public:
    virtual void Render(RenderPass renderPass, bool renderChildren = true);

    template <class T = GameObject, class... Args>
    static T* Create(Args... args);
    static void Destroy(GameObject *gameObject);

    bool IsEnabled(bool recursive = false) const;

    void SetName(const String &m_name);
    const String& GetName() const;

    template <class T>
    T* AddComponent(int index = -1);
    Component* AddComponent(const String &componentClassName, int _index = -1);
    Component* AddComponent(Component *c, int index = -1);

    static GameObject *Find(const String &name);
    GameObject *FindInChildren(const String &name, bool recursive = true);

    const List<GameObject*>& GetChildren() const;
    List<GameObject*> GetChildrenRecursively() const;

    void SetAsChild(GameObject *child);
    bool IsChildOf(const GameObject *_parent, bool recursive = true) const;

    GameObject* GetChild(int index) const;
    GameObject* GetChild(const GUID &guid) const;
    GameObject* GetChild(const String &name) const;

    template <class T>
    T* GetComponent() const;
    Component* GetComponentByGUID(const GUID &guid) const;

    template <class T>
    List<T*> GetComponents() const;
    const List<Component*>& GetComponents() const;

    template <class T>
    T* GetComponentInParent(bool recursive = true) const;

    template <class T>
    List<T*> GetComponentsInParent(bool recursive = true) const;

    template <class T>
    T* GetComponentInChildren(bool recursive = true) const;

    template <class T>
    List<T*> GetComponentsInChildren(bool recursive = true) const;

    template <class T>
    T* GetComponentInChildrenOnly(bool recursive = true) const;
    template <class T>
    List<T*> GetComponentsInChildrenOnly(bool recursive = true) const;

    template <class T>
    bool HasComponent() const;

    template <class T>
    int CountComponents() const;

    template <class T>
    void RemoveComponent();
    void RemoveComponent(Component *c);
    void RemoveComponentInstantly(Component *c);
    void RemoveQueuedComponents();

    Transform *GetTransform() const;

    void SetParent(GameObject *newParent, int _index = -1);
    GameObject* GetParent() const;

    void SetDontDestroyOnLoad(bool dontDestroyOnLoad);

    bool IsDontDestroyOnLoad() const;
    Rect GetBoundingScreenRect(Camera *cam, bool includeChildren = true) const;
    AABox GetObjectAABBox(bool includeChildren = true) const;
    AABox GetAABBox(bool includeChildren = true) const;
    Sphere GetObjectBoundingSphere(bool includeChildren = true) const;
    Sphere GetBoundingSphere(bool includeChildren = true) const;

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // IToString
    void Print(const String& indent = "") const;
    String ToStringStructure(bool recursive = false, const String &indent = "");
    String ToString() const;

    // Serializable
    virtual String GetInstanceId() const override;
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

protected:
    List<GameObject*> m_children;
    List<Component*> m_components;

    String m_name = "";
    bool m_dontDestroyOnLoad = false;

    Transform *p_transform = nullptr;
    GameObject* p_parent = nullptr;

    GameObject(const String &name = "GameObject");
    virtual ~GameObject();

    virtual void PreUpdate();
    virtual void Update();
    virtual void PostUpdate();
    virtual void BeforeChildrenRender(RenderPass renderPass);
    virtual void RenderGizmos();
    virtual void AfterChildrenRender(RenderPass renderPass);
    virtual void ChildAdded(GameObject *addedChild);
    virtual void ChildRemoved(GameObject *removedChild);
    virtual void ParentChanged(GameObject *oldParent, GameObject *newParent);

    virtual void OnEnabled() override;
    virtual void OnDisabled() override;

private:
    void PropagateEnabledEvent(bool enabled) const;

    friend class Scene;
    friend class Prefab;
    friend class GEngine;
    friend class Component;
    friend class SceneManager;
    friend class RectTransform;
};

NAMESPACE_BANG_END

#include "GameObject.tcc"

#endif // GAMEOBJECT_H
