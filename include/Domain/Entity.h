#ifndef ENTITY_H
#define ENTITY_H

#include <list>
#include <string>
#include <iostream>
#include <functional>

#include "Part.h"
#include "StageEventListener.h"

class Entity : public StageEventListener
{
friend class Stage;

private:
    std::string name;
    std::list<Part*> parts;
    std::list<Entity*> children;
    Entity* parent;

    virtual void _OnStart() override;
    virtual void _OnUpdate() override;
    virtual void _OnRender() override;
    virtual void _OnDestroy() override;

public:
    Entity();
    virtual ~Entity();

    template <class T>
    T* AddPart()
    {
        T *part = new T();
        parts.push_back(part);
        part->owner = this;
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

    //Entity* AddChild(c);
    //void GetChild(const std::string &name);
    //void RemoveChild(const std::string &name);
    //void SetParent(const std::string &name);

    const std::string GetName() const { return name; }
    const std::list<Part*>* GetParts() const { return &parts; }
    const std::list<Entity*>* GetChildren() const { return &children; }
};

#endif // ENTITY_H
