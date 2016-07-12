#include "Behaviour.h"

#include "BehaviourHolder.h"

Behaviour::Behaviour()
{
}

Behaviour::~Behaviour()
{
}

void Behaviour::OnUpdate()
{
    Time::s_deltaTime = SingletonManager::GetInstance()->GetTimeSingleton()->m_deltaTime;
}

void Behaviour::Init(BehaviourHolder *bh)
{
    p_behaviourHolder = bh;
    p_gameObject = bh->gameObject;
    p_transform = bh->gameObject->transform;
}
