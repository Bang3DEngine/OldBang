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

void Behaviour::_OnStart()
{
    RefreshBehaviourLib();
    if (!IsStarted())
    {
        Component::_OnStart();
    }
}

void Behaviour::_OnUpdate()
{
    RefreshBehaviourLib(); // For instances created in runtime

    // Update static local Times
    Time::deltaTime = Time::GetDeltaTime();
    Time::time      = Time::GetNow();

    Component::_OnUpdate();
}

void Behaviour::Read(const XMLNode &xmlInfo)
{
    // Needed to avoid a bug when creating new behaviours
    if (xmlInfo.GetTagName().Empty()) { return; }

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

Behaviour *Behaviour::CreateDynamicBehaviour(const String &behaviourName,
                                             QLibrary *openLibrary)
{
    QLibrary *lib = openLibrary;
    if (!lib) { return nullptr; }

    String errorString = "";
    if (lib->isLoaded())
    {
        // Get the pointer to the CreateDynamically function
        String funcName = "CreateDynamically_" + behaviourName;
        Behaviour* (*createFunction)(Application*) =
                (Behaviour* (*)(Application*)) (
                    lib->resolve(funcName.ToCString()));

        if (createFunction)
        {
            // Call it and get the pointer to the created Behaviour
            // Create the Behaviour, passing to it the Application
            // of this main binary, so it can link it.
            return createFunction(Application::GetInstance());
        }
        else
        {
            errorString = lib->errorString();
        }
    }
    else
    {
        errorString = lib->errorString();
    }

    Debug_Error("Error loading the library " << openLibrary->fileName()
                << "." << errorString);
    return nullptr;
}

bool Behaviour::DeleteDynamicBehaviour(const String &behaviourName,
                                       Behaviour *behaviour,
                                       QLibrary *openLibrary)
{
    if (!openLibrary) { return false; }

    // Get the pointer to the DeleteDynamically function
    String funcName = "DeleteDinamically_" + behaviourName;
    void (*deleteFunction)(Behaviour*) =
            (void (*)(Behaviour*)) (openLibrary->resolve(funcName.ToCString()));

    if (deleteFunction)
    {
        deleteFunction(behaviour);
        return true;
    }
    else
    {
        Debug_Error(openLibrary->errorString());
        return false;
    }
}

void Behaviour::SetSourceFilepath(const Path &sourceFilepath)
{
    ENSURE(GetSourceFilepath() != sourceFilepath);

    m_sourceFilepath = sourceFilepath;
}

void Behaviour::CloneInto(ICloneable *clone) const
{
    Component::CloneInto(clone);
    Behaviour *b = SCAST<Behaviour*>(clone);
    b->SetSourceFilepath( GetSourceFilepath() );
    b->p_behavioursLibraryBeingUsed = p_behavioursLibraryBeingUsed;
}

const Path &Behaviour::GetSourceFilepath() const
{
    return m_sourceFilepath;
}

void Behaviour::RefreshBehaviourLib(const XMLNode *xmlInfoForNewBehaviour)
{
    ENSURE(gameObject);
    ENSURE(!IsLoaded());

    String behaviourName = GetSourceFilepath().GetName();
    ENSURE(!behaviourName.Empty());

    // Create new Behaviour, and replace in the parent gameObject this old
    // behaviour with the new one created dynamically
    QLibrary *behavioursLib = BehaviourManager::GetBehavioursLibrary();
    p_behavioursLibraryBeingUsed = behavioursLib;
    ENSURE(behavioursLib && behavioursLib->isLoaded());

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

bool Behaviour::IsLoaded() const
{
    QLibrary *behavioursLib = BehaviourManager::GetBehavioursLibrary();
    return GetSourceFilepath().Exists() &&
           p_behavioursLibraryBeingUsed == behavioursLib &&
           GetSourceFilepath().GetName() == GetClassName();
}
