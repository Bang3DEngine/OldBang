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
}

void Behaviour::_OnUpdate()
{
    ISceneEventListener::_OnUpdate();

    // Update static Time::deltaTime variable with mainBinary deltaTime
    Time::s_deltaTime = SingletonManager::GetInstance()->GetTimeSingleton()->m_deltaTime;
}

void Behaviour::Init(BehaviourHolder *bh)
{
    m_behaviourHolder = bh;
    m_parent = bh->gameObject->GetParent();
    m_gameObject = bh->gameObject;
    m_transform = bh->gameObject->GetTransform();
}
