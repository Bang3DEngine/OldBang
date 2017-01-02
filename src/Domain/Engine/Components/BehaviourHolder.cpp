#include "BehaviourHolder.h"

#include <QLibrary>
#include <QInputDialog>

#include "XMLNode.h"
#include "Behaviour.h"
#include "FileWriter.h"
#include "Persistence.h"
#include "SystemUtils.h"
#include "BehaviourManager.h"

BehaviourHolder::BehaviourHolder()
{
}

BehaviourHolder::~BehaviourHolder()
{
    if (m_behaviour)
    {
        delete m_behaviour;
    }
    BehaviourManager::OnBehaviourHolderDeleted(this);
}

void BehaviourHolder::ChangeBehaviour(Behaviour *newBehaviour)
{
    if (m_behaviour)
    {
        delete m_behaviour;
    }
    m_behaviour = newBehaviour;
}


const String BehaviourHolder::ToString() const
{
    return "BehaviourHolder ( " + m_sourceFilepath + ")";
}

String BehaviourHolder::GetName() const
{
    return "BehaviourHolder";
}

void BehaviourHolder::CloneInto(ICloneable *clone) const
{
    Component::CloneInto(clone);
    BehaviourHolder *bh = static_cast<BehaviourHolder*>(clone);
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

void BehaviourHolder::Refresh()
{
    ASSERT(gameObject);
    #ifdef BANG_EDITOR // No refresh on temporary gameObjects
    ASSERT(!gameObject->IsDraggedGameObject());
    #endif

    String absPath = Persistence::ToAbsolute(m_sourceFilepath, false);
    ASSERT(Persistence::ExistsFile(absPath));
    BehaviourManager::Load(this, absPath);
}


void BehaviourHolder::OnBehaviourLibraryAvailable(QLibrary *lib)
{
    ASSERT(gameObject);
    ASSERT(m_currentLoadedLibrary != lib); // Its the same lib, no need to do anything

    // Create new Behaviour
    Behaviour *createdBehaviour = SystemUtils::CreateDynamicBehaviour(lib);
    m_currentLoadedLibrary = lib;

    // Change to newly created or nullptr, depending on success
    ChangeBehaviour(createdBehaviour);

    if (createdBehaviour)
    {
        if (m_behaviour)
        {
            m_behaviour->Init(this);
        }
    }
    else
    {
        Debug_Error("Behaviour " << m_sourceFilepath <<
                     " could not be refreshed. See errors above");
    }
}

void BehaviourHolder::ReadXMLInfo(const XMLNode *xmlInfo)
{
    Component::ReadXMLInfo(xmlInfo);
    String lastFilepath = m_sourceFilepath;
    m_sourceFilepath = xmlInfo->GetString("BehaviourScript");
    if (lastFilepath != m_sourceFilepath &&
        gameObject->IsInsideScene())
    {
        Refresh();
    }
}

void BehaviourHolder::FillXMLInfo(XMLNode *xmlInfo) const
{
    Component::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName("BehaviourHolder");

    xmlInfo->SetFilepath("BehaviourScript", m_sourceFilepath, "cpp");

    BehaviourHolder *noConstThis = const_cast<BehaviourHolder*>(this);
    xmlInfo->SetButton("CreateNew...", noConstThis);
}


#ifdef BANG_EDITOR
void BehaviourHolder::OnInspectorXMLNeeded(XMLNode *xmlInfo) const
{
    FillXMLInfo(xmlInfo);
}

void BehaviourHolder::OnInspectorXMLChanged(const XMLNode *xmlInfo)
{
    ReadXMLInfo(xmlInfo);
    Refresh();
}
#endif

void BehaviourHolder::OnButtonClicked(const String &attrName)
{
    #ifdef BANG_EDITOR
    if (StringUtils::Contains(attrName, "Create"))
    {
        bool ok;
        // Get Behaviour Class Name using a Dialog
        QString text = QInputDialog::getText(EditorWindow::GetInstance()->GetMainWindow(),
                                             QString("Behaviour class name"),
                                             QString("Behaviour name:"),
                                             QLineEdit::Normal,
                                             QString("MyBehaviourName"),
                                             &ok
                                             );
        if (ok)
        {
            String currentDir = Explorer::GetInstance()->GetCurrentDir();
            String className = text.toStdString();

            // Create header file
            String headerCode = Behaviour::s_behaviourHeaderTemplate;
            StringUtils::Replace(&headerCode, "CLASS_NAME", className);
            String headerFilepath = currentDir + "/" + className + ".h";
            FileWriter::WriteToFile(headerFilepath, headerCode);

            // Create source file
            String sourceCode = Behaviour::s_behaviourSourceTemplate;
            StringUtils::Replace(&sourceCode, "CLASS_NAME", className);
            String sourceFilepath = currentDir + "/" + className + ".cpp";
            FileWriter::WriteToFile(sourceFilepath, sourceCode);

            // Update Behaviour file
            m_sourceFilepath = sourceFilepath;
            Refresh();

            // Open with system editor
            // TODO: Make cross-platform
            SystemUtils::SystemBackground("xdg-open " + headerFilepath);
            SystemUtils::SystemBackground("xdg-open " + sourceFilepath);
        }
    }
    #endif
}

void BehaviourHolder::OnAddedToGameObject()
{
    Component::OnAddedToGameObject();
    Refresh();
}

void BehaviourHolder::_OnStart()
{
    Component::_OnStart();
    if (m_behaviour)
    {
        m_behaviour->m_isStarted = false;
        m_behaviour->_OnStart();
    }
    m_isStarted = true;
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
