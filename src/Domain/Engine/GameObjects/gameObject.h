#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Bang.h"

#include <list>
#include <string>
#include <iostream>
#include <functional>

#include "IFileable.h"
#include "IToString.h"
#include "ISceneEventListener.h"

#include "Material.h"

#ifdef BANG_EDITOR
#include "IWindowEventManagerListener.h"
#endif

class Component;
class Scene;
class gameObject : public ISceneEventListener,
               public IToString,
               public IFileable
               #ifdef BANG_EDITOR
                ,public IWindowEventManagerListener
               #endif
{
friend class Canvas;
friend class Prefab;
friend class Scene;
private:

    #ifdef BANG_EDITOR
    //To keep track when changing materials between selected/nonselected
    Material *nonSelectedMaterial = nullptr;
    Material *selectedMaterial = nullptr;
    #endif

    virtual void _OnStart() override;
    virtual void _OnUpdate() override;
    virtual void _OnRender() override;
    virtual void _OnDestroy() override;


    void AddChildWithoutNoifyingHierarchy(gameObject *child);
    std::list<gameObject*>::iterator RemoveChildWithoutNotifyingHierarchy(
            std::list<gameObject*>::iterator &it);
    std::list<gameObject*>::iterator RemoveChild(std::list<gameObject*>::iterator &it);

protected:
    std::string name;
    std::list<Component*> comps;
    std::list<gameObject*> children;
    gameObject* parent;

    bool enabled = true;
    bool isScene;

public:

    gameObject();
    gameObject(const std::string &name);

    virtual ~gameObject();

    void AddChild(gameObject *child);
    gameObject* GetChild(const std::string &name) const;
    void MoveChild(gameObject *child, gameObject *newParent);
    void RemoveChild(const std::string &name);
    void RemoveChild(gameObject *child);

    void SetParent(gameObject *parent);
    void SetName(const std::string &name);

    const std::string ToString() const;

    Scene* GetScene();
    gameObject* GetParent() const;
    const std::string GetName() const { return name; }
    const std::list<Component*> GetComponents() const { return comps; }
    const std::list<gameObject*> GetChildren() const { return children; }

    void AddComponent(Component *p);
    void MoveComponent(Component *p, int distance); //Mainly used to move the comps
                                          //up(-1) and down(1) in the inspector
    void RemoveComponent(Component *p);

    template <class T>
    T* AddComponent()
    {
        T *comp = new T();
        comps.push_back(comp);
        comp->owner = this;
        return comp;
    }

    template <class T>
    T* GetComponent() const
    {
        for(auto comp = comps.begin(); comp != comps.end(); ++comp)
        {
            T *tp = dynamic_cast<T*>(*comp);
            if(tp != nullptr)
            {
                return tp;
            }
        }
        return nullptr;
    }

    template <class T>
    bool HasComponent() const
    {
        return GetComponent<T>() != nullptr;
    }

    template <class T>
    void RemoveComponent()
    {
        for(auto comp = comps.begin(); comp != comps.end(); ++comp)
        {
            T *tp = dynamic_cast<T*>(*comp);
            if(tp != nullptr)
            {
                comps.erase(comp);
                delete tp;
                break;
            }
        }
    }


    bool IsScene() const;

    void Write(std::ostream &f) const override;
    void Read(std::istream &f) override;

    void SetEnabled(bool enabled) { this->enabled = enabled; }
    bool IsEnabled() { return enabled; }

    #ifdef BANG_EDITOR
    void OnTreeHierarchyEntitiesSelected(const std::list<gameObject*> &selectedEntities) override;
    #endif
};

#endif // GAMEOBJECT_H
