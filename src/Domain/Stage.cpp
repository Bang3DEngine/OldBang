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

void Stage::OnStart()
{
    for(auto e = entities.begin(); e != entities.end(); ++e)
    {
        Entity* ent = *e;
        ent->_OnStart();
    }
}

void Stage::OnUpdate()
{
    for(auto e = entities.begin(); e != entities.end(); ++e)
    {
        Entity* ent = *e;
        ent->_OnUpdate();
    }
}

void Stage::OnDestroy()
{
    for(auto e = entities.begin(); e != entities.end(); ++e)
    {
        Entity* ent = *e;
        ent->_OnDestroy();
    }
}
