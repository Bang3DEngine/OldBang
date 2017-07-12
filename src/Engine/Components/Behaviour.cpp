#include "Bang/Behaviour.h"

#include "Bang/Time.h"
#include "Bang/Dialog.h"
#include "Bang/EditorState.h"
#include "Bang/QtProjectManager.h"
#include "Bang/SingletonManager.h"

#ifdef BANG_EDITOR
#include "Bang/Explorer.h"
#include "Bang/Inspector.h"
#endif

Behaviour::Behaviour()
{
}

Behaviour::~Behaviour()
{
}

void Behaviour::_OnStart()
{
    RefreshBehaviourLib();
    #ifdef BANG_EDITOR
    if (EditorState::IsPlaying())
    #endif
    {
        if (!IsStarted())
        {
            Component::_OnStart();
        }
    }
}

void Behaviour::_OnUpdate()
{
    RefreshBehaviourLib(); // For instances created in runtime

    // Update static Time::deltaTime variable with mainBinary deltaTime
    Time::s_deltaTime = Time::GetInstance()->m_deltaTime;
    Time::s_time = Time::GetInstance()->m_time;
    Component::_OnUpdate();
}

#ifdef BANG_EDITOR
void Behaviour::OnEditorUpdate()
{
    Component::OnEditorUpdate();
    if (m_refreshInspectorRequested)
    {
        Inspector::GetInstance()->Refresh(gameObject);
    }

    RefreshBehaviourLib();
}
#endif

void Behaviour::Read(const XMLNode &xmlInfo)
{
    // Needed to avoid a bug when creating new behaviours
    if (xmlInfo.GetTagName().Empty()) { return; }

    Component::Read(xmlInfo);
    SetSourceFilepath( xmlInfo.GetFilepath("BehaviourScript") );
    m_behaviourVariablesInitValues = xmlInfo;

    RefreshBehaviourLib(&xmlInfo);
}

void Behaviour::Write(XMLNode *xmlInfo) const
{
    Component::Write(xmlInfo);
    xmlInfo->SetTagName("Behaviour");
    xmlInfo->SetFilepath("BehaviourScript", GetSourceFilepath(), "cpp");

    #ifdef BANG_EDITOR
    BehaviourId bid(GetSourceFilepath());
    bool beingCompiled = BehaviourManager::GetStatus().IsBeingCompiled(bid);
    bool failed = BehaviourManager::GetStatus().HasFailed(bid);

    Behaviour *noConstThis = const_cast<Behaviour*>(this);
    xmlInfo->SetButton("CreateNew...", noConstThis);

    if (!beingCompiled && !failed)
    {
        m_refreshInspectorRequested =
                (m_stateInInspector != StateInInspector::Normal);
        m_stateInInspector = StateInInspector::Normal;
    }
    else if (beingCompiled)
    {
        m_refreshInspectorRequested =
                (m_stateInInspector != StateInInspector::BeingCompiled);
        m_stateInInspector = StateInInspector::BeingCompiled;
    }
    else if (failed)
    {
        m_refreshInspectorRequested =
                (m_stateInInspector != StateInInspector::Failed);
        m_stateInInspector = StateInInspector::Failed;
    }
    #endif
}

#ifdef BANG_EDITOR
void Behaviour::OnButtonClicked(const AttrWidgetButton *clickedButton)
{
    /*
    if (attrName.Contains("Create"))
    {
        CreateNewBehaviour();
    }
    */
}

Behaviour* Behaviour::CreateNewBehaviour()
{
    bool ok;
    // Get Behaviour Class Name using a Dialog
    String className = Dialog::GetInputString("Behaviour class name",
                                              "Behaviour name:",
                                              "MyBehaviourName",
                                              &ok );

    Behaviour *newBehaviour = nullptr;
    if (ok && !className.Empty())
    {
        newBehaviour = new Behaviour();
        Path currentDir = Explorer::GetInstance()->GetCurrentDir();

        // Create header file
        String headerCode = Behaviour::s_behaviourHeaderTemplate;
        headerCode = headerCode.Replace("CLASS_NAME", className);
        Path headerFilepath = currentDir.Append(className);
        headerFilepath = headerFilepath.AppendExtension("h");
        if (headerFilepath.IsFile()) { return nullptr; }
        File::Write(headerFilepath, headerCode);

        // Create source file
        String sourceCode = Behaviour::s_behaviourSourceTemplate;
        sourceCode = sourceCode.Replace("CLASS_NAME", className);
        Path sourceFilepath = currentDir.Append(className);
        sourceFilepath = sourceFilepath.AppendExtension("cpp");
        if (sourceFilepath.IsFile()) { return nullptr; }
        File::Write(sourceFilepath, sourceCode);

        // Update Behaviour file
        newBehaviour->SetSourceFilepath( sourceFilepath );
        newBehaviour->RefreshBehaviourLib();

        // Open with system editor
        QtProjectManager::OpenBehaviourInQtCreator(sourceFilepath);
        QtProjectManager::CreateQtProjectFile(); // Refresh it
    }

    return newBehaviour;
}
#endif


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
        Behaviour* (*createFunction)(SingletonManager*) =
                (Behaviour* (*)(SingletonManager*)) (
                    lib->resolve(funcName.ToCString()));

        if (createFunction)
        {
            // Call it and get the pointer to the created Behaviour
            // Create the Behaviour, passing to it the SingletonManager
            // of this main binary, so it can link it.
            return createFunction(SingletonManager::s_mainBinarySM);
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

    XMLNode empty;
    m_behaviourVariablesInitValues = empty;
}

void Behaviour::CloneInto(ICloneable *clone) const
{
    Component::CloneInto(clone);
    Behaviour *b = Object::SCast<Behaviour>(clone);
    b->SetSourceFilepath( GetSourceFilepath() );
    b->m_behaviourVariablesInitValues = m_behaviourVariablesInitValues;
    b->p_behavioursLibraryBeingUsed   = p_behavioursLibraryBeingUsed;
}

const Path &Behaviour::GetSourceFilepath() const
{
    return m_sourceFilepath;
}

void Behaviour::RefreshBehaviourLib(const XMLNode *xmlInfoForNewBehaviour)
{
    ENSURE(gameObject);
    ENSURE(!IsLoaded());
    #ifdef BANG_EDITOR
    ENSURE(!gameObject->IsDraggedGameObject());
    ENSURE(BehaviourManager::GetStatus().IsReady(GetSourceFilepath()));
    #endif

    String behaviourName = GetSourceFilepath().GetName();
    ENSURE(!behaviourName.Empty());

    // Create new Behaviour, and replace in the parent gameObject this old
    // behaviour with the new one created dynamically
    QLibrary *behavioursLib = BehaviourManager::GetBehavioursMergedLibrary();
    p_behavioursLibraryBeingUsed = behavioursLib;
    ENSURE(behavioursLib && behavioursLib->isLoaded());

    Behaviour *createdBehaviour = CreateDynamicBehaviour(behaviourName,
                                                         behavioursLib);
    if (createdBehaviour)
    {
        if (gameObject->AddComponent(createdBehaviour))
        {
            CloneInto(createdBehaviour);
            createdBehaviour->Read(m_behaviourVariablesInitValues);
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
    QLibrary *behavioursLib =  BehaviourManager::GetBehavioursMergedLibrary();
    return GetSourceFilepath().Exists() &&
           p_behavioursLibraryBeingUsed == behavioursLib &&
           GetSourceFilepath().GetName() == GetClassName();
}


String Behaviour::s_behaviourHeaderTemplate = R"VERBATIM(
#ifndef CLASS_NAME_H
#define CLASS_NAME_H

#include "Bang/Behaviour.h"
#include ".CLASS_NAME.refl.h"

// Here go your includes

BP_REFLECT_CLASS(CLASS_NAME)
class CLASS_NAME : public Behaviour
{
    OBJECT(CLASS_NAME);
public:
    void OnStart() override;
    void OnUpdate() override;

    BP_REFLECT_DEFINITIONS(CLASS_NAME)
};

#endif // CLASS_NAME_H

BANG_BEHAVIOUR_CLASS(CLASS_NAME)
)VERBATIM"
;

String Behaviour::s_behaviourSourceTemplate = R"VERBATIM(
#include "CLASS_NAME.h"

// This function will be executed once when created
void CLASS_NAME::OnStart()
{
    Behaviour::OnStart();
}

// This function will be executed every frame
void CLASS_NAME::OnUpdate()
{
    Behaviour::OnUpdate();
}

BANG_BEHAVIOUR_CLASS_IMPL(CLASS_NAME);
)VERBATIM"
;
