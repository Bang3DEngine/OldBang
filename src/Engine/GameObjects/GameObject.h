#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <queue>

#include "Bang/List.h"
#include "Bang/AABox.h"
#include "Bang/Object.h"
#include "Bang/Sphere.h"
#include "Bang/IToString.h"
#include "Bang/SerializableObject.h"
#include "Bang/ISceneEventListener.h"

class Scene;
class Material;
class Component;
class Transform;
class GameObject : public ISceneEventListener
                  ,public IToString
                  ,public SerializableObject
{
    OBJECT_NO_FRIEND(GameObject)

public:
    // PROPERTIES
    String const& name   = m_name;
    GameObject* const& parent = p_parent;
    Transform* const& transform = m_transform;
    //

    GameObject(const String &m_name = "GameObject");

    virtual void CloneInto(ICloneable *clone) const override;

    virtual ~GameObject();

    void SetParent(GameObject *parent, bool keepWorldTransform = false,
                   GameObject *aboveThisChild = nullptr);

    GameObject* GetChild(const String &m_name) const;

    void SetName(const String &m_name);

    void Print(const String& indent = "") const;

    String ToString() const;

    GameObject* GetParent() const;
    const String& GetName() const;
    const List<Component*>& GetComponents() const;

    static void Destroy(GameObject *gameObject);

    /**
     * @brief GetChildren
     * @return
     */
    const List<GameObject*>& GetChildren() const;

    /**
     * @brief GetChildrenRecursively
     * @return
     */
    List<GameObject*> GetChildrenRecursively() const;

    /**
     * @brief Returns the bounding screen rect of the gameObject rendered
     * from the passed camera in NDC.
     * @return
     */
    Rect GetBoundingScreenRect(Camera *cam,
                               bool includeChildren = true) const;

    /**
     * @brief Returns this GameObject's bounding box in Object space, without
     * applying any Transform (equivalent to Mesh->GetBoundingBox())
     * @return
     */
    AABox GetObjectAABBox(bool includeChildren = true) const;

    /**
     * @brief Returns this GameObject's bounding box in world space
     * @return
     */
    AABox GetAABBox(bool includeChildren = true) const;

    /**
     * @brief Returns this GameObject's bounding sphere in Object space, without
     * applying any Transform (equivalent to Mesh->GetBoundingBox())
     * @return
     */
    Sphere GetObjectBoundingSphere(bool includeChildren = true) const;

    /**
     * @brief Returns this GameObject's bounding sphere in world space
     * @return
     */
    Sphere GetBoundingSphere(bool includeChildren = true) const;

    /**
     * Adds the Component c to this.
     */
    bool AddComponent(Component *c);

    /**
     * Creates a Component of type T, adds it to this,
     * and returns a pointer to it
     */
    template <class T>
    T* AddComponent()
    {
        T *c = new T();
        if (!AddComponent(c)) { return nullptr; }
        if (IsStarted()) { std::cerr << "============   c->_OnStart()" << std::endl; c->_OnStart(); }
        return c;
    }

    /**
     * Returns the first Component<T> found in this
     */
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

    /**
     * Returns all the Components<T> in this
     */
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

    /**
     * Returns all the Components<T> of it and its children
     */
    template <class T>
    List<T*> GetComponentsInThisAndChildren() const
    {
        List<T*> comps = GetComponentsInChildren<T>();
        List<T*> thisComps = GetComponents<T>();
        thisComps.Splice(thisComps.End(), comps);
        return thisComps;
    }

    /**
     * Returns whether this has one or more Components of type T or not
     */
    template <class T>
    bool HasComponent() const
    {
        return GetComponent<T>() ;
    }


    /**
     * Returns the number of Components of type T
     */
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

    /**
     * Removes the Component c
     */
    void RemoveComponent(Component *c);
    void RemoveComponentInstantly(Component *c);
    void RemoveQueuedComponents();

    /**
     * Removes the first found Component of type T
     */
    template <class T>
    void RemoveComponent()
    {
        for (Component *comp : m_components)
        {
            T *compT = DCAST<T*>(comp);
            if (compT)
            {
                RemoveComponent(compT);
                break;
            }
        }
    }

    Transform* GetTransform() const;

    virtual void OnMouseEnter(bool fromChildren);
    virtual void OnMouseOver(bool fromChildren);
    virtual void OnMouseExit(bool fromChildren);

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

    void ChangeTransformByRectTransform();

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

    virtual void _OnStart () override;
    virtual void _OnUpdate () override;
    virtual void _OnDrawGizmos(Gizmos::GizmosPassType gizmosPassType) override;
    virtual void _OnDestroy () override;

    friend class Scene;
    friend class Prefab;
    friend class SceneManager;
    friend class GPPass_G_Gizmos;
    friend class GraphicPipeline;
};

#endif // GAMEOBJECT_H
