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
        ent->_OnUpdate();
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
