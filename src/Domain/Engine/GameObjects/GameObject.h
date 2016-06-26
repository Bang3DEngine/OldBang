#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Bang.h"

#include <list>
#include <string>
#include <iostream>
#include <functional>

#include "IFileable.h"
#include "IToString.h"
#include "ICloneable.h"
#include "ISceneEventListener.h"

#include "Material.h"
#include "Sphere.h"
#include "Box.h"

#ifdef BANG_EDITOR
#include "IWindowEventManagerListener.h"
#endif

class Scene;
class Component;
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

friend class Hierarchy;
friend class Canvas;
friend class Prefab;
friend class Scene;

private:

    #ifdef BANG_EDITOR
    bool ed_wasSelectedInHierarchy = false;
    EditorSelectionGameObject *ed_selectionGameObject = nullptr;
    #endif

    virtual void _OnStart () override;
    virtual void _OnUpdate () override;
    virtual void _OnPreRender () override;
    virtual void _OnRender () override;
    virtual void _OnDestroy () override;

protected:
    std::string name = "";
    std::list<Component*> comps;
    std::list<GameObject*> children;
    GameObject* parent = nullptr;

    /**
     * @brief A RenderLayer is the order in which gameObjects will
     * be drawn on screen. First all GameObjects on layer 0 are drawn, then
     * all GameObjects on layer 1, etc. GameObjects rendered on layer N will
     * not take into account the depth in layer N-1.
     *  (the depth buffer will be cleared after completely rendering each layer.)
     */
    unsigned char renderLayer = 0;

    bool enabled = true;
    bool isScene = false;

    void AddChildWithoutNotifyingHierarchy(GameObject *child);
    std::list<GameObject*>::iterator RemoveChildWithoutNotifyingHierarchy(
            std::list<GameObject*>::iterator &it);
    std::list<GameObject*>::iterator RemoveChild(
            std::list<GameObject*>::iterator &it);

public:

    GameObject();
    GameObject(const std::string &name);
    GameObject(const GameObject &go);
    virtual void CloneInto(ICloneable *clone) const override;
    virtual ICloneable *Clone() const override;

    virtual ~GameObject();

    void AddChild(GameObject *child);
    GameObject* GetChild(const std::string &name) const;
    void MoveChild(GameObject *child, GameObject *newParent);
    void RemoveChild(const std::string &name);
    void RemoveChild(GameObject *child);

    void SetParent(GameObject *parent);
    void SetRenderLayer(unsigned char layer);
    void SetName(const std::string &name);

    const std::string ToString() const;


    Scene* GetScene();
    GameObject* GetParent() const;
    const std::string GetName() const;
    unsigned char GetRenderLayer() const;
    const std::list<Component*>& GetComponents() const;

    /**
     * TODO: Change so this method returns a reference, putting
     *       editorGameObjects into another list...
     * @brief GetChildren
     * @return
     */
    const std::list<GameObject*> GetChildren() const;

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

    /**
     * Adds the Component c to this.
     */
    void AddComponent(Component *c);

    /**
     * Moves the Component c upwards(-N), or downwards(N).
     * Used only for Inspector showing order purposes
     */
    void MoveComponent(Component *c, int distance);

    /**
     * Removes the Component c
     */
    void RemoveComponent(Component *c);

    /**
     * Creates a Component of type T, adds it to this,
     * and returns a pointer to it
     */
    template <class T>
    T* AddComponent()
    {
        T *comp = new T();
        AddComponent(comp);
        return comp;
    }

    /**
     * Returns the first Component<T> found in this
     */
    template <class T>
    T* GetComponent() const
    {
        for(auto comp = comps.begin(); comp != comps.end(); ++comp)
        {
            T *tp = dynamic_cast<T*>(*comp);
            if(tp ) return tp;
        }
        return nullptr;
    }

    /**
     * Returns all the Components<T> in this
     */
    template <class T>
    std::list<T*> GetComponents() const
    {
        std::list<T*> comps_l;
        for(auto comp = comps.begin(); comp != comps.end(); ++comp)
        {
            T *tp = dynamic_cast<T*>(*comp);
            if(tp ) comps_l.push_back(tp);
        }
        return comps_l;
    }

    /**
     * Returns the first Component<T> found in children
     */
    template <class T>
    T* GetComponentInChildren() const
    {
        for(auto c = children.begin(); c != children.end(); ++c)
        {
            if((*c)->IsEditorGameObject()) continue;

            Component *comp = (*c)->GetComponent<T>();
            if(comp ) return comp;
            comp = (*c)->GetComponentInChildren<T>();
            if(comp ) return comp;
        }
        return nullptr;
    }

    /**
     * Returns all the Components<T> of its children
     */
    template <class T>
    std::list<T*> GetComponentsInChildren() const
    {
        std::list<T*> comps_l;
        for(auto c = children.begin(); c != children.end(); ++c)
        {
            comps_l.splice(comps_l.end(),
                           (*c)->GetComponents<T>()); //concat
            comps_l.splice(comps_l.end(),
                           (*c)->GetComponentsInChildren<T>()); //concat
        }
        return comps_l;
    }

    /**
     * Returns all the Components<T> of this,
     * and all the Components<T> of its children
     */
    template <class T>
    std::list<T*> GetComponentsInThisAndChildren() const
    {
        std::list<T*> comps_l = GetComponents<T>();
        comps_l.splice(comps_l.end(),
                       GetComponentsInChildren<T>()); //concat
        return comps_l;
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
     * Removes the first found Component of type T
     */
    template <class T>
    void RemoveComponent()
    {
        for(auto comp = comps.begin(); comp != comps.end(); ++comp)
        {
            T *tp = dynamic_cast<T*>(*comp);
            if(tp )
            {
                comps.erase(comp);
                delete tp;
                break;
            }
        }
    }

    virtual void OnMouseEnter() {}
    virtual void OnMouseExit() {}

    virtual bool IsEditorGameObject() const;
    bool IsScene() const;

    void Write(std::ostream &f) const override;
    void Read(std::istream &f) override;

    void SetEnabled(bool enabled);
    bool IsEnabled();

    #ifdef BANG_EDITOR
    void OnTreeHierarchyGameObjectsSelected(
            std::list<GameObject*> &selectedEntities) override;
    #endif
};

#endif // GAMEOBJECT_H
