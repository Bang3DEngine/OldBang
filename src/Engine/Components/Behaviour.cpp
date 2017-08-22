#include "Bang/Behaviour.h"

#include "Bang/Time.h"
#include "Bang/Application.h"
#include "Bang/BehaviourManager.h"

Behaviour::Behaviour()
{
}

Behaviour::~Behaviour()
{
}

void Behaviour::RefreshBehaviourLib(const XMLNode *xmlInfoForNewBehaviour)
{
    ENSURE(gameObject);
    ENSURE(!IsLoaded());

    String behaviourName = GetSourceFilepath().GetName();
    ENSURE(!behaviourName.IsEmpty());

    // Create new Behaviour, and replace in the parent gameObject this old
    // behaviour with the new one created dynamically
    Library *behavioursLib = BehaviourManager::GetBehavioursLibrary();
    p_behavioursLibraryBeingUsed = behavioursLib;
    ENSURE(behavioursLib && behavioursLib->IsLoaded());

    Behaviour *createdBehaviour = CreateDynamicBehaviour(behaviourName,
                                                         behavioursLib);
    if (createdBehaviour)
    {
        if (gameObject->AddComponent(createdBehaviour))
        {
            CloneInto(createdBehaviour);
            if (xmlInfoForNewBehaviour)
            {
                createdBehaviour->Read(*xmlInfoForNewBehaviour);
            }
            gameObject->RemoveComponent(this);
        }
    }
}

Behaviour *Behaviour::CreateDynamicBehaviour(const String &behaviourName,
                                             Library *openLibrary)
{
    Library *lib = openLibrary;
    if (!lib) { return nullptr; }

    String errorString = "";
    if (lib->IsLoaded())
    {
        // Get the pointer to the CreateDynamically function
        String funcName = "CreateDynamically_" + behaviourName;
        Behaviour* (*createFunction)(Application*) =
            lib->Get< Behaviour*(*)(Application*) >(funcName.ToCString());

        if (createFunction)
        {
            // Call it and get the pointer to the created Behaviour
            // Create the Behaviour, passing to it the Application
            // of this main binary, so it can link it.
            return createFunction(Application::GetInstance());
        }
        else { errorString = lib->GetErrorString(); }
    }
    else { errorString = lib->GetErrorString(); }

    Debug_Error(errorString);
    return nullptr;
}

bool Behaviour::DeleteDynamicBehaviour(const String &behaviourName,
                                       Behaviour *behaviour,
                                       Library *openLibrary)
{
    if (!openLibrary) { return false; }

    // Get the pointer to the DeleteDynamically function
    String funcName = "DeleteDinamically_" + behaviourName;
    void (*deleteFunction)(Behaviour*) =
            (openLibrary->Get<void (*)(Behaviour*)>(funcName.ToCString()));

    if (deleteFunction)
    {
        deleteFunction(behaviour);
        return true;
    }
    else
    {
        Debug_Error(openLibrary->GetErrorString());
        return false;
    }
}

void Behaviour::SetSourceFilepath(const Path &sourceFilepath)
{
    ENSURE(GetSourceFilepath() != sourceFilepath);
    m_sourceFilepath = sourceFilepath;
}

const Path &Behaviour::GetSourceFilepath() const { return m_sourceFilepath; }
bool Behaviour::IsLoaded() const
{
    Library *behavioursLib = BehaviourManager::GetBehavioursLibrary();
    return GetSourceFilepath().Exists() &&
           p_behavioursLibraryBeingUsed == behavioursLib &&
           GetSourceFilepath().GetName() == GetClassName();
}



void Behaviour::Start()
{
    RefreshBehaviourLib();
    if (!IsStarted())
    {
        Component::Start();
    }
}

void Behaviour::Update()
{
    RefreshBehaviourLib(); // For instances created in runtime

    // Update static local Times
    Time::deltaTime = Time::GetDeltaTime();
    Time::time      = Time::GetNow();

    Component::Update();
}

void Behaviour::CloneInto(ICloneable *clone) const
{
    Component::CloneInto(clone);
    Behaviour *b = SCAST<Behaviour*>(clone);
    b->SetSourceFilepath( GetSourceFilepath() );
    b->p_behavioursLibraryBeingUsed = p_behavioursLibraryBeingUsed;
}

void Behaviour::Read(const XMLNode &xmlInfo)
{
    // Needed to avoid a bug when creating new behaviours
    if (xmlInfo.GetTagName().IsEmpty()) { return; }

    Component::Read(xmlInfo);
    SetSourceFilepath( xmlInfo.Get<Path>("BehaviourScript") );

    RefreshBehaviourLib(&xmlInfo);
}

void Behaviour::Write(XMLNode *xmlInfo) const
{
    Component::Write(xmlInfo);
    xmlInfo->SetTagName("Behaviour");
    xmlInfo->Set("BehaviourScript", GetSourceFilepath());
}
