#include "Behaviour.h"

#include "BehaviourHolder.h"

Behaviour::Behaviour()
{
}

Behaviour::~Behaviour()
{
}

void Behaviour::Init(BehaviourHolder *bh)
{
    p_behaviourHolder = bh;
    gameObject = bh->gameObject;
}
