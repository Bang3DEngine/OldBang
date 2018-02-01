#include "Bang/BehaviourContainer.h"

#include "Bang/XMLNode.h"
#include "Bang/Behaviour.h"
#include "Bang/Application.h"
#include "Bang/BehaviourManager.h"

USING_NAMESPACE_BANG

BehaviourContainer::BehaviourContainer()
{
}

BehaviourContainer::~BehaviourContainer()
{
}

void BehaviourContainer::OnPreStart()
{
    Component::OnPreStart();
    TryToSubstituteByBehaviourInstance();
}

void BehaviourContainer::OnGameObjectChanged()
{
    Component::OnGameObjectChanged();
    TryToSubstituteByBehaviourInstance();
}

void BehaviourContainer::SetSourceFilepath(const Path &sourceFilepath)
{
    if (sourceFilepath != GetSourceFilepath())
    {
        m_sourceFilepath = sourceFilepath;
    }
}

Behaviour *BehaviourContainer::CreateBehaviourInstance(Library *behavioursLibrary) const
{
    return BehaviourManager::CreateBehaviourInstance(GetBehaviourName(),
                                                     behavioursLibrary);
}

String BehaviourContainer::GetBehaviourName() const
{
    return GetSourceFilepath().GetName();
}

const Path &BehaviourContainer::GetSourceFilepath() const
{
    return m_sourceFilepath;
}

void BehaviourContainer::TryToSubstituteByBehaviourInstance()
{
    BehaviourManager *behaviourManager = BehaviourManager::GetActive();
    if ( behaviourManager &&
         behaviourManager->IsInstanceCreationAllowed() &&
        !IsWaitingToBeDestroyed() &&
        !GetBehaviourName().IsEmpty()
       )
    {
        Library *behLib = behaviourManager->GetBehavioursLibrary();
        SubstituteByBehaviourInstance(behLib);
    }
}

void BehaviourContainer::SubstituteByBehaviourInstance(Library *behavioursLibrary)
{
    Behaviour *behaviour = CreateBehaviourInstance(behavioursLibrary);
    if (behaviour)
    {
        GetGameObject()->AddComponent(behaviour);
        Component::Destroy(this);
    }
}

void BehaviourContainer::CloneInto(ICloneable *clone) const
{
    Component::CloneInto(clone);
    BehaviourContainer *bc = Cast<BehaviourContainer*>(clone);
    bc->SetSourceFilepath( GetSourceFilepath() );
}

void BehaviourContainer::ImportXML(const XMLNode &xmlInfo)
{
    Component::ImportXML(xmlInfo);

    if (xmlInfo.Contains("SourceFilepath"))
    { SetSourceFilepath( xmlInfo.Get<Path>("SourceFilepath") ); }

    TryToSubstituteByBehaviourInstance();
}

void BehaviourContainer::ExportXML(XMLNode *xmlInfo) const
{
    Component::ExportXML(xmlInfo);

    xmlInfo->Set("SourceFilepath", GetSourceFilepath());
}
