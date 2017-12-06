#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <queue>
#include <stack>
#include <tuple>

#include "Bang/List.h"
#include "Bang/Object.h"
#include "Bang/IToString.h"
#include "Bang/RenderPass.h"
#include "Bang/IsContainer.h"
#include "Bang/Serializable.h"
#include "Bang/IEventEmitter.h"
#include "Bang/ObjectManager.h"
#include "Bang/IDestroyListener.h"
#include "Bang/IChildrenListener.h"
#include "Bang/GameObjectFactory.h"

NAMESPACE_BANG_BEGIN

FORWARD class Camera;
FORWARD class Component;
FORWARD class RectTransform;

#define GAMEOBJECT(ClassName) \
    public: virtual ClassName* Clone() const override {\
        ClassName *clone = GameObject::Create<ClassName>();\
        CloneInto(clone);\
        return clone;\
    }\
    SERIALIZABLE(ClassName) \
    friend class ObjectManager;

class GameObject : public Object,
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

    void SetAsChild(GameObject *child);
    void SetParent(GameObject *newParent, int _index = -1);
    void SetDontDestroyOnLoad(bool dontDestroyOnLoad);

    GameObject* GetChild(int index) const;
    GameObject* GetChild(const GUID &guid) const;
    GameObject* GetChild(const String &name) const;
    const List<GameObject*>& GetChildren() const;
    List<GameObject*> GetChildrenRecursively() const;

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
    bool HasComponent(const String &className) const;

    void RemoveComponent(Component *component);
    Scene* GetScene() const;
    Transform *GetTransform() const;
    RectTransform *GetRectTransform() const;
    GameObject* GetParent() const;
    bool IsChildOf(const GameObject *_parent, bool recursive = true) const;

    bool IsDontDestroyOnLoad() const;
    Rect GetBoundingScreenRect(Camera *cam, bool includeChildren = true) const;
    AABox GetObjectAABBox(bool includeChildren = true) const;
    AABox GetAABBox(bool includeChildren = true) const;
    Sphere GetObjectBoundingSphere(bool includeChildren = true) const;
    Sphere GetBoundingSphere(bool includeChildren = true) const;

    // Helper propagate functions
    template<class T>
    static bool CanEventBePropagated(const T& x);

    // Propagation to a single non-event listener
    template<class TFunction, class T, class... Args>
    static typename std::enable_if<
        (std::is_pointer<T>::value || std::is_reference<T>::value) &&
        !std::is_base_of<IEventListener,
                             typename std::remove_pointer<T>::type>::value &&
        !IsContainer<T>::value, void >::type
    Propagate(const TFunction &func, const T &obj, const Args&... args);

    // Propagation to a single event listener
    template<class TFunction, class T, class... Args>
    static typename std::enable_if<
        (std::is_pointer<T>::value || std::is_reference<T>::value) &&
         std::is_base_of<IEventListener,
                         typename std::remove_pointer<T>::type>::value &&
         !IsContainer<T>::value, void >::type
    Propagate(const TFunction &func, const T &obj, const Args&... args);

    // List propagation
    template<class TFunction, template <class T> class TContainer, class T, class... Args>
    static typename std::enable_if<
        (std::is_pointer<T>::value || std::is_reference<T>::value) &&
         IsContainer<TContainer<T>>::value, void >::type
    Propagate(const TFunction &func, const TContainer<T> &container, const Args&... args);

    template<class TFunction, class... Args>
    void PropagateToChildren(const TFunction &func, const Args&... args);

    template<class TFunction, class... Args>
    void PropagateToComponents(const TFunction &func, const Args&... args);


    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // IToString
    void Print(const String& indent = "") const;
    String ToStringStructure(bool recursive = false, const String &indent = "");
    String ToString() const override;

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

    // Concurrent modification when iterating stuff
    bool m_increaseChildrenIterator = true;
    bool m_increaseComponentsIterator = true;
    std::stack< List<GameObject*>::Iterator > m_currentChildrenIterators;
    std::stack< List<Component*>::Iterator  > m_currentComponentsIterators;

    GameObject(const String &name = "GameObject");
    virtual ~GameObject();

    virtual void PreUpdate();
    virtual void BeforeChildrenUpdate();
    virtual void Update();
    virtual void AfterChildrenUpdate();
    virtual void PostUpdate();
    virtual void BeforeChildrenRender(RenderPass renderPass);
    virtual void RenderGizmos();
    virtual void AfterChildrenRender(RenderPass renderPass);
    virtual void ChildAdded(GameObject *addedChild);
    virtual void ChildRemoved(GameObject *removedChild);
    virtual void ParentChanged(GameObject *oldParent, GameObject *newParent);

    // IEnabledListener
    virtual void OnEnabled() override;
    virtual void OnDisabled() override;

private:
    void PropagateEnabledEvent(bool enabled) const;

    void AddChild(GameObject *child, int index);
    void RemoveChild(GameObject *child);

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
