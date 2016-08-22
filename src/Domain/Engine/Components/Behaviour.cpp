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
    m_gameObject = bh->gameObject;
    m_parent = m_gameObject->parent;
    m_transform = m_gameObject->transform;
}

String Behaviour::s_behaviourHeaderTemplate =
"\
#ifndef CLASS_NAME_H \n\
#define CLASS_NAME_H \n\
\n\
#include \"Behaviour.h\" \n\
\n\
class CLASS_NAME : public Behaviour \n\
{ \n\
public: \n\
 \n\
    void OnStart() override; \n\
    void OnUpdate() override; \n\
}; \n\
\n\
BANG_BEHAVIOUR_CLASS(CLASS_NAME) \n\
\n\
#endif // CLASS_NAME_H \n\
"
;

String Behaviour::s_behaviourSourceTemplate =
"\
#include \"CLASS_NAME.h\" \n\
\n\
void CLASS_NAME::OnStart() \n\
{ \n\
    Behaviour::OnStart(); \n\
    // This function will be executed once when created \n\
} \n\
\n\
void CLASS_NAME::OnUpdate() \n\
{ \n\
    Behaviour::OnUpdate(); \n\
    // This function will be executed every frame \n\
} \n\
\
"
;
