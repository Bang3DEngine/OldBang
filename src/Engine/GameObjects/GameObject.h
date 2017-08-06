#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <queue>

#include "Bang/List.h"
#include "Bang/IToString.h"
#include "Bang/ComponentFactory.h"
#include "Bang/SerializableObject.h"
#include "Bang/ISceneEventListener.h"

FORWARD class Scene;
FORWARD class Camera;
FORWARD class Material;
FORWARD class Component;

class GameObject : public ISceneEventListener,
                   public IToString,
                   public SerializableObject
{
    OBJECT(GameObject)

public:
    String const& name = m_name;
    GameObject* const& parent = p_parent;
    Transform* const& transform = m_transform;

    GameObject(const String &m_name = "GameObject");

    virtual void CloneInto(ICloneable *clone) const override;

    virtual ~GameObject();

    void SetParent(GameObject *parent, int index = -1);

    GameObject* GetChild(int index) const;
    GameObject* GetChild(const String &m_name) const;

    void SetName(const String &m_name);

    void Print(const String& indent = "") const;
    String ToString() const;

    GameObject* GetParent() const;
    const String& GetName() const;
    const List<Component*>& GetComponents() const;

    static void Destroy(GameObject *gameObject);

    const List<GameObject*>& GetChildren() const;
    List<GameObject*> GetChildrenRecursively() const;

    Rect GetBoundingScreenRect(Camera *cam, bool includeChildren = true) const;

    AABox GetObjectAABBox(bool includeChildren = true) const;
    AABox GetAABBox(bool includeChildren = true) const;
    Sphere GetObjectBoundingSphere(bool includeChildren = true) const;
    Sphere GetBoundingSphere(bool includeChildren = true) const;

    Component* AddComponent(const String &componentClassName, int _index = -1);
    Component* AddComponent(Component *c, int index = -1);
    template <class T>
    T* AddComponent(int index = -1)
    {
        T *c = ComponentFactory::CreateComponent<T>();
        this->AddComponent(c, index);
        return c;
    }

    template <class T>
    T* GetComponent() const
    {
        for (Component *comp : m_components)
        {
            T *tp = DCAST<T*>(comp);
            if (tp) { return tp; }
        }
        return nullptr;
    }

    template <class T>
    List<T*> GetComponents() const
    {
        List<T*> comps_l;
        for (Component *comp : m_components)
        {
            T *tp = DCAST<T*>(comp);
            if (tp) { comps_l.PushBack(tp); }
        }
        return comps_l;
    }

    template <class T>
    T* GetComponentInParent() const
    {
        if (!parent) { return nullptr; }
        T* comp = parent->GetComponent<T>();
        if (comp) { return comp; }
        return parent->GetComponentInParent<T>();
    }

    template <class T>
    List<T*> GetComponentsInParent() const
    {
        List<T*> result;
        if (!parent) { return result; }
        result = parent->GetComponents<T>();
        return result.Concat(parent->GetComponentsInParent<T>());
    }


    /**
     * Returns the first Component<T> found in children
     */
    template <class T>
    T* GetComponentInChildren() const
    {
        for (auto c = m_children.Begin(); c != m_children.End(); ++c)
        {
            T *comp = (*c)->GetComponent<T>();
            if (comp) return comp;
            comp = (*c)->GetComponentInChildren<T>();
            if (comp) return comp;
        }
        return nullptr;
    }

    /**
     * Returns all the Components<T> of its children
     */
    template <class T>
    List<T*> GetComponentsInChildren() const
    {
        List<T*> comps_l;
        for (auto c = m_children.Begin(); c != m_children.End(); ++c)
        {
            List<T*> childComps = (*c)->GetComponents<T>();
            comps_l.Splice(comps_l.End(), childComps); //concat
            List<T*> childChildrenComps = (*c)->GetComponentsInChildren<T>();
            comps_l.Splice(comps_l.End(), childChildrenComps); //concat
        }
        return comps_l;
    }

    template <class T>
    List<T*> GetComponentsInThisAndChildren() const
    {
        List<T*> comps = GetComponentsInChildren<T>();
        List<T*> thisComps = GetComponents<T>();
        thisComps.Splice(thisComps.End(), comps);
        return thisComps;
    }

    template <class T>
    bool HasComponent() const
    {
        return GetComponent<T>() ;
    }

    template <class T>
    int CountComponents() const
    {
        int count = 0;
        for (auto comp = m_components.Begin(); comp != m_components.End(); ++comp)
        {
            T *tp = DCAST<T*>(comp);
            if (tp) { ++count; }
        }
        return count;
    }

    void RemoveComponent(Component *c);
    void RemoveComponentInstantly(Component *c);
    void RemoveQueuedComponents();
    template <class T>
    void RemoveComponent()
    {
        for (Component *comp : m_components)
        {
            T *compT = DCAST<T*>(comp);
            if (compT)
            {
                this->RemoveComponent(compT);
                break;
            }
        }
    }

    bool IsChildOf(const GameObject *parent, bool recursive = true) const;

    static GameObject *Find(const String &name);
    GameObject *FindInChildren(const String &name, bool recursive = true);

    void UpdateXMLInfo(const XMLNode &xmlInfo);
    void ReadFirstTime(const XMLNode &xmlInfo);
    virtual String GetInstanceId() const override;
    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

    void SetEnabled(bool m_enabled);
    bool IsEnabled() const;

protected:
    String m_name = "";
    List<Component*> m_components;
    List<GameObject*> m_children;
    Transform *m_transform = nullptr;
    GameObject *p_parent = nullptr;

    std::queue<Component*> m_componentsToBeRemoved;

    bool m_enabled = true;
    bool m_hasBeenReadOnce = false;
    bool m_iteratingComponents = false;

    virtual void Start() override;
    virtual void Update() override;
    virtual void ParentSizeChanged() override;
    virtual void DrawGizmos() override;
    virtual void Destroy() override;

    friend class Scene;
    friend class Prefab;
    friend class SceneManager;
    friend class RectTransform;
    friend class GraphicPipeline;
};

#endif // GAMEOBJECT_H
