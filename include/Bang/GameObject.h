#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <queue>

#include "Bang/List.h"
#include "Bang/Object.h"
#include "Bang/IToString.h"
#include "Bang/RenderPass.h"
#include "Bang/Serializable.h"

NAMESPACE_BANG_BEGIN

FORWARD class Camera;
FORWARD class Component;

#define GAMEOBJECT(ClassName) SERIALIZABLE(ClassName)

class GameObject : public Object,
                   public Serializable,
                   public IToString
{
    GAMEOBJECT(GameObject)

public:
    String const& name = m_name;
    GameObject* const& parent = p_parent;
    Transform* const& transform = p_transform;

    virtual ~GameObject();

    virtual void Start() override;
    virtual void Update();
    virtual void BeforeChildrenRender(RenderPass renderPass);
    virtual void ChildrenRendered(RenderPass renderPass);
    virtual void Render(RenderPass renderPass, bool renderChildren = true);
    virtual void ParentSizeChanged();
    virtual void RenderGizmos();
    virtual void Destroy();

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

    bool IsChildOf(const GameObject *_parent, bool recursive = true) const;

    GameObject* GetChild(int index) const;
    GameObject* GetChild(const GUID &guid) const;
    GameObject* GetChild(const String &name) const;

    template <class T> // To enable auto-complete
    T* GetComponent() const { return _GetComponent<T>(); }
    Component* GetComponentByGUID(const GUID &guid) const;

    template <class T>
    List<T*> GetComponents() const;
    const List<Component*>& GetComponents() const;

    template <class T>
    T* GetComponentInParent() const;

    template <class T>
    List<T*> GetComponentsInParent() const;

    template <class T>
    T* GetComponentInChildren() const;

    template <class T>
    List<T*> GetComponentsInChildren() const;

    template <class T>
    bool HasComponent() const;

    template <class T>
    int CountComponents() const;

    template <class T>
    void RemoveComponent();
    void RemoveComponent(Component *c);
    void RemoveComponentInstantly(Component *c);
    void RemoveQueuedComponents();

    void SetParent(GameObject *newParent, int _index = -1);
    GameObject* GetParent();
    const GameObject* GetParent() const;

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

    ::std::queue<Component*> m_componentsToBeRemoved;

    bool m_iteratingComponents = false;

private:
    GameObject(const String &m_name = "GameObject");

    template <class T> T* _GetComponent() const;

    friend class Scene;
    friend class Prefab;
    friend class UIGameObject;
    friend class SceneManager;
    friend class RectTransform;
    friend class GraphicPipeline;
    friend class GameObjectFactory;
};

NAMESPACE_BANG_END

#include "GameObject.tcc"

#endif // GAMEOBJECT_H
