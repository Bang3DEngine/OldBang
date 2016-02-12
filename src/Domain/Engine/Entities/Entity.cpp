#include "Entity.h"
#include "Stage.h"
#include "Part.h"

Entity::Entity() : Entity("")
{
}

Entity::Entity(const std::string &name) : name(name), parent(nullptr)
{
}

Entity::~Entity()
{
    _OnDestroy();
}


Stage *Entity::GetStage() const
{
    if(parent != nullptr)
    {
        Stage *st = dynamic_cast<Stage*>(parent);
        if(st == nullptr) return parent->GetStage();
        else return st;
    }
    return nullptr;
}

Entity *Entity::GetParent() const
{
    return parent;
}

void Entity::AddChild(Entity *child)
{
    child->parent = this;
    children.push_back(child);
}

Entity *Entity::GetChild(const std::string &name) const
{
    for(auto it = children.begin(); it != children.end(); ++it)
    {
        Entity *child = (*it);
        if(child->name == name)
        {
            return child;
        }
    }
    return nullptr;
}

void Entity::RemoveChild(const std::string &name)
{
    for(auto it = children.begin(); it != children.end(); ++it)
    {
        Entity *child = (*it);
        if(child->name == name)
        {
            child->parent = nullptr;
            children.erase(it);
            return;
        }
    }
}

void Entity::SetParent(Entity *newParent)
{
    if(parent != nullptr)
        parent->RemoveChild(name);

    if(newParent != nullptr)
        newParent->AddChild(this);
}

const std::string Entity::ToString() const
{
    return " [Entity:'" + name + "']";
}

void Entity::_OnStart()
{
    OnStart();
    PROPAGATE_EVENT(_OnStart, parts);
    PROPAGATE_EVENT(_OnStart, children);
}


void Entity::_OnUpdate()
{
    OnUpdate();
    PROPAGATE_EVENT(_OnUpdate, parts);
    PROPAGATE_EVENT(_OnUpdate, children);
}

void Entity::_OnRender()
{
    OnRender();
    PROPAGATE_EVENT(_OnRender, parts);
    PROPAGATE_EVENT(_OnRender, children);
}

void Entity::_OnDestroy()
{
    OnDestroy();
    PROPAGATE_EVENT(_OnDestroy, parts);
    PROPAGATE_EVENT(_OnDestroy, children);
}
