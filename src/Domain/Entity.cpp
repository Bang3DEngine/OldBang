#include "Entity.h"

Entity::Entity() : parent(nullptr)
{
}

Entity::~Entity()
{
    _OnDestroy();
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
