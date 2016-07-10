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
    this->behaviourHolder = bh;
    gameObject = bh->gameObject;
}

void Behaviour::SetSourceFilepath(const std::string &sourceFilepath)
{
    this->sourceFilepath = sourceFilepath;
}

std::string Behaviour::GetSourceFilepath() const
{
    return sourceFilepath;
}
