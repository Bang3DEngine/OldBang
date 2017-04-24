#include "Bang/Behaviour.h"

#include "Bang/Time.h"
#include "Bang/Dialog.h"
#include "Bang/EditorState.h"
#include "Bang/QtProjectManager.h"
#include "Bang/SingletonManager.h"

#ifdef BANG_EDITOR
#include "Bang/Explorer.h"
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
        Component::_OnStart();
    }
}

void Behaviour::_OnUpdate()
{
    RefreshBehaviourLib(); // For instances created in runtime
    #ifdef BANG_EDITOR
    if (EditorState::IsPlaying())
    #endif
    {
        // Update static Time::deltaTime variable with mainBinary deltaTime
        Time::s_deltaTime = Time::GetInstance()->m_deltaTime;
        Time::s_time = Time::GetInstance()->m_time;
        Component::_OnUpdate();
    }
}

void Behaviour::Read(const XMLNode &xmlInfo)
{
    Component::Read(xmlInfo);
    m_sourceFilepath = xmlInfo.GetString("BehaviourScript");
    RefreshBehaviourLib();
}

void Behaviour::Write(XMLNode *xmlInfo) const
{
    Component::Write(xmlInfo);
    xmlInfo->SetTagName("Behaviour");
    xmlInfo->SetFilepath("BehaviourScript", GetSourceFilepath(), "cpp");

    Behaviour *noConstThis = const_cast<Behaviour*>(this);
    xmlInfo->SetButton("CreateNew...", noConstThis);
}

#ifdef BANG_EDITOR
void Behaviour::OnButtonClicked(const String &attrName)
{
    if (attrName.Contains("Create"))
    {
        CreateNewBehaviour();
    }
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
        // TODO: Check that the class name is a valid name!
        String currentDir = Explorer::GetInstance()->GetCurrentDir();

        // Create header file
        String headerCode = Behaviour::s_behaviourHeaderTemplate;
        headerCode = headerCode.Replace("CLASS_NAME", className);
        String headerFilepath = currentDir + "/" + className;
        headerFilepath = IO::AppendExtension(headerFilepath, "h");
        if (IO::ExistsFile(headerFilepath)) { return nullptr; }
        IO::WriteToFile(headerFilepath, headerCode);

        // Create source file
        String sourceCode = Behaviour::s_behaviourSourceTemplate;
        sourceCode = sourceCode.Replace("CLASS_NAME", className);
        String sourceFilepath = currentDir + "/" + className;
        sourceFilepath = IO::AppendExtension(sourceFilepath, "cpp");
        if (IO::ExistsFile(sourceFilepath)) { return nullptr; }
        IO::WriteToFile(sourceFilepath, sourceCode);

        // Update Behaviour file
        newBehaviour->m_sourceFilepath = sourceFilepath;
        newBehaviour->RefreshBehaviourLib();

        // Open with system editor
        // TODO: Make cross-platform
        QtProjectManager::OpenBehaviourInQtCreator(headerFilepath);
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

void Behaviour::CloneInto(ICloneable *clone) const
{
    Component::CloneInto(clone);
    Behaviour *b = Object::SCast<Behaviour>(clone);
    b->m_sourceFilepath = GetSourceFilepath();
    b->p_behavioursLibraryBeingUsed = p_behavioursLibraryBeingUsed;
}

const String &Behaviour::GetSourceFilepath() const
{
    return m_sourceFilepath;
}

void Behaviour::RefreshBehaviourLib()
{
    ENSURE(gameObject);
    ENSURE(!IsLoaded());
    #ifdef BANG_EDITOR
    ENSURE(!gameObject->IsDraggedGameObject());
    #endif

    String behaviourName = IO::GetFileName(GetSourceFilepath());
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
            gameObject->RemoveComponent(this);
        }
    }
}

bool Behaviour::IsLoaded() const
{
    String absPath = IO::ToAbsolute(GetSourceFilepath(), false);
    QLibrary *behavioursLib =  BehaviourManager::GetBehavioursMergedLibrary();
    return IO::ExistsFile(absPath) &&
           p_behavioursLibraryBeingUsed == behavioursLib &&
           IO::GetFileName( GetSourceFilepath() ) == GetClassName();
}

void Behaviour::OnAddedToGameObject()
{
}


String Behaviour::s_behaviourHeaderTemplate =
"\
#ifndef CLASS_NAME_H \n\
#define CLASS_NAME_H \n\
\n\
#include \"Bang/Behaviour.h\" \n\
\n\
// Here go your includes \n\
\n\
class CLASS_NAME : public Behaviour \n\
{ \n\
OBJECT(CLASS_NAME) \n\
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
