#include "Behaviour.h"

#include "BehaviourHolder.h"

Behaviour::Behaviour()
{
}

Behaviour::~Behaviour()
{
}

String Behaviour::GetBehaviourName() const
{
    String bName = "";
    if (p_behaviourHolder)
    {
        const String srcFilepath = p_behaviourHolder->GetSourceFilepath();
        bName = Persistence::GetFileName(srcFilepath);
    }
    return bName;
}

void Behaviour::OnUpdate()
{
}

void Behaviour::_OnUpdate()
{
    ISceneEventListener::_OnUpdate();

    // Update static Time::deltaTime variable with mainBinary deltaTime
    Time::s_deltaTime = SingletonManager::Get<Time>()->m_deltaTime;
    Time::s_time = SingletonManager::Get<Time>()->m_time;
}

void Behaviour::Init(BehaviourHolder *bh)
{
    p_behaviourHolder = bh;
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
// Here go your includes \n\
\n\
class CLASS_NAME : public Behaviour \n\
{ \n\
public: \n\
    void OnStart() override; \n\
    void OnUpdate() override; \n\
}; \n\
\n\
#endif // CLASS_NAME_H \n\
\n\
BANG_BEHAVIOUR_CLASS(CLASS_NAME);\
"
;

String Behaviour::s_behaviourSourceTemplate =
"\
#include \"CLASS_NAME.h\" \n\
\n\
// This function will be executed once when created \n\
void CLASS_NAME::OnStart() \n\
{ \n\
    Behaviour::OnStart(); \n\
} \n\
\n\
// This function will be executed every frame \n\
void CLASS_NAME::OnUpdate() \n\
{ \n\
    Behaviour::OnUpdate(); \n\
} \n\
\n\
BANG_BEHAVIOUR_CLASS_IMPL(CLASS_NAME);\
"
;
