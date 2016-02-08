#include "Stage.h"


Stage::Stage()
{
}

Stage::~Stage()
{
    for(auto e = entities.begin(); e != entities.end(); ++e)
    {
        Entity *ent = *e;
        ent->_OnDestroy();
        delete ent;
    }
}

Entity *Stage::AddEntity(const std::string &name)
{
    Entity *ent =  new Entity();
    ent->name = name;
    entities.push_back(ent);
    return ent;
}

Entity *Stage::GetEntity(const std::string &name) const
{
    for(auto it = entities.begin(); it != entities.end(); ++it)
    {
        if((*it)->name == name)
        {
            return (*it);
        }
    }
    return nullptr;
}

void Stage::_OnStart()
{
    OnStart();
    for(auto e = entities.begin(); e != entities.end(); ++e)
    {
        Entity* ent = *e;
        ent->_OnStart();
    }
}

void Stage::_OnUpdate()
{
    OnUpdate();
    for(auto e = entities.begin(); e != entities.end(); ++e)
    {
        Entity* ent = *e;
        ent->_OnUpdate();
    }
}

void Stage::_OnDrawing()
{
    OnDrawing();
    for(auto e = entities.begin(); e != entities.end(); ++e)
    {
        Entity* ent = *e;
        ent->_OnDrawing();
    }
}


void Stage::_OnDestroy()
{
    OnDestroy();
    for(auto e = entities.begin(); e != entities.end(); ++e)
    {
        Entity* ent = *e;
        ent->_OnDestroy();
    }
}


