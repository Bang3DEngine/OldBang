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

void Behaviour::SetSourceFilepath(const std::string &sourceFilepath)
{
    m_sourceFilepath = sourceFilepath;
}

std::string Behaviour::GetSourceFilepath() const
{
    return m_sourceFilepath;
}
