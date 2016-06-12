#include "Behaviour.h"

#include "BehaviourHolder.h"

Behaviour::Behaviour()
{
}

Behaviour::~Behaviour()
{
}

GameObject *Behaviour::GetOwner() const
{
    if(behaviourHolder == nullptr) return nullptr;
    return behaviourHolder->GetOwner();
}

void Behaviour::SetSourceFilepath(const std::string &filepath)
{
    this->sourceFilepath = sourceFilepath;
}

std::string Behaviour::GetSourceFilepath() const
{
    return sourceFilepath;
}
