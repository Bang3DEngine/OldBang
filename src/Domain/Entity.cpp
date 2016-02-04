#include "Entity.h"

Entity::Entity() : parent(nullptr)
{
}

Entity::~Entity()
{
    for(auto part = parts.begin(); part != parts.end(); ++part)
    { //Propagate to parts
        (*part)->_OnDestroy();
    }

    for(auto child = children.begin(); child != children.end(); ++child)
    { //Propagate to children
        (*child)->_OnDestroy();
    }
}

void Entity::_OnStart()
{
    OnStart();

    for(auto part = parts.begin(); part != parts.end(); ++part)
    { //location >= 0 to parts
        (*part)->_OnStart();
    }

    for(auto child = children.begin(); child != children.end(); ++child)
    { // to children
        (*child)->_OnStart();
    }
}


void Entity::_OnUpdate()
{
    OnUpdate();

    for(auto part = parts.begin(); part != parts.end(); ++part)
    {
        (*part)->_OnUpdate();
    }

    for(auto child = children.begin(); child != children.end(); ++child)
    {
        (*child)->_OnUpdate();
    }
}

void Entity::_OnDestroy()
{
    OnDestroy();

    for(auto part = parts.begin(); part != parts.end(); ++part)
    {
        (*part)->_OnDestroy();
    }

    for(auto child = children.begin(); child != children.end(); ++child)
    {
        (*child)->_OnDestroy();
    }
}
