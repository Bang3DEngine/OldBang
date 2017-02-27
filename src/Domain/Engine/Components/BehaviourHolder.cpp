#include "BehaviourHolder.h"

#include <QLibrary>
#include <QInputDialog>

#include "XMLNode.h"
#include "Behaviour.h"
#include "FileWriter.h"
#include "Persistence.h"
#include "SystemUtils.h"
#include "EditorState.h"
#include "BehaviourManager.h"
#include "QtProjectManager.h"

BehaviourHolder::BehaviourHolder()
{
}

BehaviourHolder::~BehaviourHolder()
{
    if (m_behaviour)
    {
        delete m_behaviour;
    }
}

void BehaviourHolder::ChangeBehaviour(Behaviour *newBehaviour)
{
    if (m_behaviour)
    {
        delete m_behaviour;
    }
    m_behaviour = newBehaviour;
}

Behaviour *BehaviourHolder::CreateDynamicBehaviour(const String &behaviourName,
                                                   QLibrary *openLibrary)
{
    QLibrary *lib = openLibrary; ASSERT(lib, "", return nullptr);
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
            return createFunction(SingletonManager::GetInstance());
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

bool BehaviourHolder::DeleteDynamicBehaviour(const String &behaviourName,
                                             Behaviour *b,
                                             QLibrary *openLibrary)
{
    if (!openLibrary) { return false; }

    // Get the pointer to the DeleteDynamically function
    String funcName = "DeleteDinamically_" + behaviourName;
    void (*deleteFunction)(Behaviour*) =
            (void (*)(Behaviour*)) (openLibrary->resolve(funcName.ToCString()));

    if (deleteFunction)
    {
        deleteFunction(b);
        return true;
    }
    else
    {
        Debug_Error(openLibrary->errorString());
        return false;
    }
}

String BehaviourHolder::GetName() const
{
    return "BehaviourHolder";
}

void BehaviourHolder::CloneInto(ICloneable *clone) const
{
    Component::CloneInto(clone);
    BehaviourHolder *bh = Object::SCast<BehaviourHolder>(clone);
    bh->m_sourceFilepath = m_sourceFilepath;
}

ICloneable *BehaviourHolder::Clone() const
{
    BehaviourHolder *bh = new BehaviourHolder();
    CloneInto(bh);
    return bh;
}

const String &BehaviourHolder::GetSourceFilepath() const
{
    return m_sourceFilepath;
}

void BehaviourHolder::RefreshBehaviourLib()
{
    ASSERT(gameObject);
    #ifdef BANG_EDITOR
    ASSERT(!gameObject->IsDraggedGameObject());
    #endif

    String absPath = Persistence::ToAbsolute(m_sourceFilepath, false);
    ASSERT(Persistence::ExistsFile(absPath));
    String behaviourName = Persistence::GetFileName(GetSourceFilepath());
    ASSERT(!behaviourName.Empty());

    QLibrary *newLib = BehaviourManager::GetBehavioursMergedLibrary();
    if (newLib && m_currentLoadedLibrary != newLib)
    {
        m_currentLoadedLibrary = newLib;

        // Create new Behaviour
        Behaviour *createdBehaviour =
                CreateDynamicBehaviour(behaviourName, m_currentLoadedLibrary);

        ChangeBehaviour(createdBehaviour);
        if (createdBehaviour)
        {
            if (m_behaviour) { m_behaviour->Init(this); }
        }
        else
        {
            Debug_Error("Behaviour " << m_sourceFilepath <<
                         " could not be refreshed. See errors above");
        }
    }
}

QLibrary *BehaviourHolder::GetLibraryBeingUsed() const
{
    return m_currentLoadedLibrary;
}

void BehaviourHolder::ReadXMLInfo(const XMLNode *xmlInfo)
{
    Component::ReadXMLInfo(xmlInfo);
    String lastFilepath = m_sourceFilepath;
    m_sourceFilepath = xmlInfo->GetString("BehaviourScript");
    if (lastFilepath != m_sourceFilepath &&
        gameObject->IsInsideScene())
    {
        RefreshBehaviourLib();
    }
}

void BehaviourHolder::FillXMLInfo(XMLNode *xmlInfo) const
{
    Component::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName("BehaviourHolder");

    xmlInfo->SetFilepath("BehaviourScript", m_sourceFilepath, "cpp");
}


#ifdef BANG_EDITOR
void BehaviourHolder::OnInspectorXMLNeeded(XMLNode *xmlInfo) const
{
    FillXMLInfo(xmlInfo);
    BehaviourHolder *noConstThis = const_cast<BehaviourHolder*>(this);
    xmlInfo->SetButton("CreateNew...", noConstThis);
}

void BehaviourHolder::OnInspectorXMLChanged(const XMLNode *xmlInfo)
{
    ReadXMLInfo(xmlInfo);
    RefreshBehaviourLib();
}

void BehaviourHolder::OnButtonClicked(const String &attrName)
{
    if (StringUtils::Contains(attrName, "Create"))
    {
        CreateNewBehaviour();
    }
}

void BehaviourHolder::CreateNewBehaviour()
{
    bool ok;
    // Get Behaviour Class Name using a Dialog
    String className = Dialog::GetInputString("Behaviour class name",
                                              "Behaviour name:",
                                              "MyBehaviourName",
                                              &ok
                                             );
    if (ok && !className.Empty())
    {
        // TODO: Check that the class name is a valid name!
        String currentDir = Explorer::GetInstance()->GetCurrentDir();

        // Create header file
        String headerCode = Behaviour::s_behaviourHeaderTemplate;
        StringUtils::Replace(&headerCode, "CLASS_NAME", className);
        String headerFilepath = currentDir + "/" + className;
        headerFilepath = Persistence::AppendExtension(headerFilepath, "h");
        ASSERT(!Persistence::ExistsFile(headerFilepath));
        Persistence::WriteToFile(headerFilepath, headerCode);

        // Create source file
        String sourceCode = Behaviour::s_behaviourSourceTemplate;
        StringUtils::Replace(&sourceCode, "CLASS_NAME", className);
        String sourceFilepath = currentDir + "/" + className;
        sourceFilepath = Persistence::AppendExtension(sourceFilepath, "cpp");
        ASSERT(!Persistence::ExistsFile(sourceFilepath));
        Persistence::WriteToFile(sourceFilepath, sourceCode);

        // Update Behaviour file
        m_sourceFilepath = sourceFilepath;
        RefreshBehaviourLib();

        // Open with system editor
        // TODO: Make cross-platform
        QtProjectManager::OpenBehaviourInQtCreator(headerFilepath);
        QtProjectManager::OpenBehaviourInQtCreator(sourceFilepath);
        QtProjectManager::CreateQtProjectFile(); // Refresh it
    }
}
#endif

void BehaviourHolder::OnAddedToGameObject()
{
    Component::OnAddedToGameObject();
    RefreshBehaviourLib();
}

void BehaviourHolder::_OnStart()
{
    Component::_OnStart();

    RefreshBehaviourLib();
    if (m_behaviour)
    {
        #ifdef BANG_EDITOR
        if (EditorState::IsPlaying())
        #endif
        {
            m_behaviour->_OnStart();
        }
    }
}

void BehaviourHolder::_OnUpdate()
{
    Component::_OnUpdate();
    if (m_behaviour)
    {
        m_behaviour->_OnUpdate();
    }
}

void BehaviourHolder::_OnDrawGizmos()
{
    #ifdef BANG_EDITOR
    Component::_OnDrawGizmos();
    #endif

    if (m_behaviour)
    {
        m_behaviour->_OnDrawGizmos();
    }
}

void BehaviourHolder::_OnDrawGizmosOverlay()
{
    #ifdef BANG_EDITOR
    Component::_OnDrawGizmosOverlay();
    #endif

    if (m_behaviour)
    {
        m_behaviour->_OnDrawGizmosOverlay();
    }
}

void BehaviourHolder::_OnDestroy()
{
    Component::_OnDestroy();
    if (m_behaviour)
    {
        m_behaviour->_OnDestroy();
    }
}
