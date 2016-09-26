#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Bang.h"

#include <iostream>
#include <functional>

#include "List.h"
#include "IFileable.h"
#include "IToString.h"
#include "ICloneable.h"
#include "ISceneEventListener.h"

#include "Box.h"
#include "Sphere.h"
#include "Property.h"
#include "Material.h"

#ifdef BANG_EDITOR
#include "Gizmos.h"
#include "IWindowEventManagerListener.h"
#endif

class Scene;
class Component;
class Transform;
class EditorSelectionGameObject;
class GameObject :
                public ISceneEventListener
               ,public IToString
               ,public IFileable
               ,public ICloneable
               #ifdef BANG_EDITOR
               ,public IWindowEventManagerListener
               #endif
{

protected:

    #ifdef BANG_EDITOR
    // Dont load Behaviours. Used on drags from hierarchy for example
    bool m_isDragged = false;

    bool m_isSelectedInHierarchy = false;
    EditorSelectionGameObject *m_selectionGameObject = nullptr;
    #endif

    virtual void _OnStart () override;
    virtual void _OnUpdate () override;
    virtual void _OnDestroy () override;

    #ifdef BANG_EDITOR
    Array<Material*> m_materialsBeforeDrag;
    void OnDragEnterMaterial(Material *m);
    void OnDragLeaveMaterial(Material *m);
    void OnDropMaterial(Material *m);
    #endif

    virtual void _OnDrawGizmos () override;
    virtual void _OnDrawGizmosOverlay () override;
    virtual void OnDrawGizmos() override;
    virtual void OnDrawGizmosOverlay() override;

protected:
    String m_name = "";
    List<Component*> m_components;
    List<GameObject*> m_children;
    Transform *m_transform = nullptr;
    GameObject *m_parent = nullptr;

    bool m_enabled = true;

public:

    // PROPERTIES
    String const& name   = m_name;
    GameObject* const& parent = m_parent;
    Transform* const& transform = m_transform;
    //

    GameObject();
    GameObject(const String &m_name);
    GameObject(const GameObject &go);

    virtual void CloneInto(ICloneable *clone) const override;
    virtual ICloneable *Clone() const override;

    virtual ~GameObject();

    void SetParent(GameObject *parent, bool keepWorldTransform = false);
    GameObject* GetChild(const String &m_name) const;

    void SetName(const String &m_name);

    const String ToString() const;

    Scene* GetScene();
    bool IsInsideScene() const;
    GameObject* GetParent() const;
    const String GetName() const;
    const List<Component*>& GetComponents() const;

    /**
     * TODO: Change so this method returns a reference, putting
     *       editorGameObjects into another list...
     * @brief GetChildren
     * @return
     */
    const List<GameObject*> GetChildren() const;

    /**
     * @brief GetChildrenRecursively
     * @return
     */
    List<GameObject*> GetChildrenRecursively() const;

    #ifdef BANG_EDITOR
    List<GameObject*> GetChildrenRecursivelyEditor() const;
    #endif


    /**
     * @brief Returns this GameObject's bounding box in Object space, without
     * applying any Transform (equivalent to Mesh->GetBoundingBox())
     * @return
     */
    Box GetObjectBoundingBox() const;

    /**
     * @brief Returns this GameObject's bounding box, applying only the
     * transformations of this GameObject's Transform
     * (not parents' transforms included here).
     * @return
     */
    Box GetLocalBoundingBox() const;

    /**
     * @brief Returns this GameObject's bounding box in world space
     * @return
     */
    Box GetBoundingBox() const;

    /**
     * @brief Returns this GameObject's bounding sphere in Object space, without
     * applying any Transform (equivalent to Mesh->GetBoundingBox())
     * @return
     */
    Sphere GetObjectBoundingSphere() const;

    /**
     * @brief Returns this GameObject's bounding sphere, applying only the
     * transformations of this GameObject's Transform
     * (not parents' transforms included here).
     * @return
     */
    Sphere GetLocalBoundingSphere() const;

    /**
     * @brief Returns this GameObject's bounding sphere in world space
     * @return
     */
    Sphere GetBoundingSphere() const;

    #ifdef BANG_EDITOR
    /**
     * Moves the Component c upwards(-N), or downwards(N).
     * Used only for Inspector showing order purposes
     */
    void MoveComponent(Component *c, int distance);
    #endif

    const List<GameObject *> GetChildrenEditor() const;

    /**
     * Removes the Component c
     */
    void RemoveComponent(Component *c);

    /**
     * Adds the Component c to this.
     */
    void AddComponent(Component *c);

    /**
     * Creates a Component of type T, adds it to this,
     * and returns a pointer to it
     */
    template <class T>
    T* AddComponent()
    {
        T *c = new T();
        AddComponent(c);
        return c;
    }

    /**
     * Returns the first Component<T> found in this
     */
    template <class T>
    T* GetComponent() const
    {
        for (auto comp = m_components.Begin(); comp != m_components.End(); ++comp)
        {
            T *tp = dynamic_cast<T*>(*comp);
            if (tp) return tp;
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
        for (auto comp = m_components.Begin(); comp != m_components.End(); ++comp)
        {
            T *tp = dynamic_cast<T*>(*comp);
            if (tp) comps_l.PushBack(tp);
        }
        return comps_l;
    }

    /**
     * Returns the first Component<T> found in children
     */
    template <class T>
    T* GetComponentInChildren() const
    {
        for (auto c = m_children.Begin(); c != m_children.End(); ++c)
        {
            if ((*c)->IsEditorGameObject()) continue;

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
            comps_l.Splice(comps_l.Begin(), childComps); //concat
            List<T*> childChildrenComps = (*c)->GetComponentsInChildren<T>();
            comps_l.Splice(comps_l.Begin(), childChildrenComps); //concat
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
        comps.Splice(comps.Begin(), thisComps);
        return comps;
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
        for (auto it = m_components.Begin(); it != m_components.End(); ++it)
        {
            T *tp = dynamic_cast<T*>(*it);
            if (tp)
            {
                ++count;
            }
        }
        return count;
    }

    /**
     * Removes the first found Component of type T
     */
    template <class T>
    void RemoveComponent()
    {
        for (auto it = m_components.Begin(); it != m_components.End(); ++it)
        {
            T *tp = dynamic_cast<T*>(*it);
            if (tp)
            {
                it = m_components.Remove(it); --it;
                delete tp;
                break;
            }
        }
    }

    Transform* GetTransform() const;

    virtual void OnMouseEnter(bool fromChildren);
    virtual void OnMouseOver(bool fromChildren);
    virtual void OnMouseExit(bool fromChildren);

    bool IsChildOf(const GameObject *parent) const;
    virtual bool IsEditorGameObject() const;
    virtual bool IsScene() const;

    static GameObject *Find(const String &name);
    GameObject *FindInChildren(const String &name);

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;

    void SetEnabled(bool m_enabled);
    bool IsEnabled() const;

    #ifdef BANG_EDITOR
    // Is it being dragged from hierarchy into screen?
    bool IsDraggedGameObject() const;

    bool IsSelectedInHierarchy() const;
    void OnTreeHierarchyGameObjectsSelected(
            List<GameObject*> &selectedEntities) override;
    #endif


    friend class Scene;
    friend class Screen;
    friend class Prefab;
    friend class Hierarchy;
    friend class SceneManager;
    friend class GraphicPipeline;
    friend class SelectionFramebuffer;
};

#endif // GAMEOBJECT_H
