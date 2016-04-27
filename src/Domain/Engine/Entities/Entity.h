#ifndef ENTITY_H
#define ENTITY_H

#include "Bang.h"

#include <list>
#include <string>
#include <iostream>
#include <functional>

#include "IFileable.h"
#include "IToString.h"
#include "IStageEventListener.h"

#ifdef BANG_EDITOR
#include "IWindowEventManagerListener.h"
#endif

class Part;
class Stage;
class Entity : public IStageEventListener,
               public IToString,
               public IFileable
               #ifdef BANG_EDITOR
                ,public IWindowEventManagerListener
               #endif
{
friend class Canvas;
friend class Prefab;
friend class Stage;
private:

    virtual void _OnStart() override;
    virtual void _OnUpdate() override;
    virtual void _OnRender() override;
    virtual void _OnDestroy() override;


    void AddChildWithoutNoifyingHierarchy(Entity *child);
    void RemoveChildWithoutNoifyingHierarchy(std::list<Entity*>::iterator &it);
    void RemoveChild(std::list<Entity*>::iterator &it);

protected:
    std::string name;
    std::list<Part*> parts;
    std::list<Entity*> children;
    Entity* parent;

    bool enabled = true;
    bool isStage;

public:

    Entity();
    Entity(const std::string &name);

    virtual ~Entity();

    void AddChild(Entity *child);
    Entity* GetChild(const std::string &name) const;
    void MoveChild(Entity *child, Entity *newParent);
    void RemoveChild(const std::string &name);
    void RemoveChild(Entity *child);

    void SetParent(Entity *parent);
    void SetName(const std::string &name);

    const std::string ToString() const;

    Stage* GetStage();
    Entity* GetParent() const;
    const std::string GetName() const { return name; }
    const std::list<Part*> GetParts() const { return parts; }
    const std::list<Entity*> GetChildren() const { return children; }

    void AddPart(Part *p);
    void MovePart(Part *p, int distance); //Mainly used to move the parts
                                          //up(-1) and down(1) in the inspector
    void RemovePart(Part *p);

    template <class T>
    T* AddPart()
    {
        T *part = new T();
        parts.push_back(part);
        part->parent = this;
        return part;
    }

    template <class T>
    T* GetPart() const
    {
        for(auto part = parts.begin(); part != parts.end(); ++part)
        {
            T *tp = dynamic_cast<T*>(*part);
            if(tp != nullptr)
            {
                return tp;
            }
        }
        return nullptr;
    }

    template <class T>
    bool HasPart() const
    {
        return GetPart<T>() != nullptr;
    }

    template <class T>
    void RemovePart()
    {
        for(auto part = parts.begin(); part != parts.end(); ++part)
        {
            T *tp = dynamic_cast<T*>(*part);
            if(tp != nullptr)
            {
                parts.erase(part);
                delete tp;
                break;
            }
        }
    }


    bool IsStage() const;

    void Write(std::ostream &f) const override;
    void Read(std::istream &f) override;

    void SetEnabled(bool enabled) { this->enabled = enabled; }
    bool IsEnabled() { return enabled; }

    #ifdef BANG_EDITOR
    void OnTreeHierarchyEntitiesSelected(const std::list<Entity*> &selectedEntities) override;
    #endif
};

#endif // ENTITY_H
