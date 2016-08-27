#include "BehaviourHolder.h"

BehaviourHolder::BehaviourHolder()
{
    m_compileThread = new CompileBehaviourThread();
    m_compileThread->SetBehaviourHolder(this);
}

BehaviourHolder::~BehaviourHolder()
{
    if (m_compileThread)
    {
        if (m_compileThread->isRunning())
        {
            m_compileThread->SetBehaviourHolder(nullptr);
        }
        else
        {
            delete m_compileThread;
        }
    }

    if (m_behaviour)
    {
        delete m_behaviour;
    }

    if (m_currentOpenLibrary)
    {
        delete m_currentOpenLibrary;
        // SystemUtils::CloseLibrary(m_currentOpenLibrary);
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
    // bh->Refresh();
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
    NONULL(gameObject);

    // No refresh on temporary gameObjects
    #ifdef BANG_EDITOR
    if (gameObject->IsDraggedGameObject()) return;
    #endif

    if (m_sourceFilepath == "") return;

    if (!m_compileThread->isRunning())
    {
        String filename = Persistence::GetFileNameWithExtension(m_sourceFilepath);
        Debug_Log("Refreshing Behaviour '" << filename << "'...");
        m_compileThread->start();
    }
}


void BehaviourHolder::OnBehaviourFinishedCompiling(String soFilepath)
{
    // Prepare thread for next compilation
    m_compileThread = new CompileBehaviourThread();

    if (soFilepath == "")
    {
        ChangeBehaviour(nullptr);
        Debug_Error("There was an error compiling the Behaviour "
                     << m_sourceFilepath);
        return;
    }

    // Create new Behaviour
    // void *openLibrary = nullptr;
    QLibrary *openLibrary = nullptr;
    Behaviour *createdBehaviour = nullptr;
    SystemUtils::CreateDynamicBehaviour(soFilepath, &createdBehaviour,
                                        &openLibrary);

    ChangeBehaviour(createdBehaviour); // To newly created or nullptr, depending on success

    if (createdBehaviour)
    {
        if (m_currentOpenLibrary)
        {
            delete m_currentOpenLibrary;
            // SystemUtils::CloseLibrary(m_currentOpenLibrary);
        }
        m_currentOpenLibrary = openLibrary;
    }

    if (createdBehaviour)
    {
        if (m_behaviour)
        {
            m_behaviour->Init(this);
            m_behaviour->_OnStart();
            String filename = Persistence::GetFileNameWithExtension(m_sourceFilepath);
            Debug_Log("Behaviour '" << filename << "' successfully refreshed!");
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
    if (m_compileThread->isRunning())
    {
        xmlInfo->SetButton("Refresh", noConstThis, {XMLProperty::Disabled});
    }
    else
    {
        xmlInfo->SetButton("Refresh", noConstThis);
    }

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
            SystemUtils::SystemBackground("xdg-open " + headerFilepath);
            SystemUtils::SystemBackground("xdg-open " + sourceFilepath);
        }
    }
    else if (attrName == "Refresh")
    {
        Refresh();
    }
#endif
}

void BehaviourHolder::_OnStart()
{
    Component::_OnStart();
    Refresh();
}

void BehaviourHolder::_OnUpdate()
{
    Component::_OnUpdate();
    if (m_behaviour)
    {
        m_behaviour->_OnUpdate();
    }
}

void BehaviourHolder::_OnPreRender()
{
    Component::_OnPreRender();
    if (m_behaviour)
    {
        m_behaviour->_OnPreRender();
    }
}

void BehaviourHolder::_OnRender()
{
    Component::_OnRender();
    if (m_behaviour)
    {
        m_behaviour->_OnRender();
    }
}

void BehaviourHolder::_OnDrawGizmos()
{
    Component::_OnDrawGizmos();
    if (m_behaviour)
    {
        m_behaviour->_OnDrawGizmos();
    }
}

void BehaviourHolder::_OnDrawGizmosNoDepth()
{
    Component::_OnDrawGizmosNoDepth();
    if (m_behaviour)
    {
        m_behaviour->_OnDrawGizmosNoDepth();
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

//////////////////////////////////////////////////////////////////////////

CompileBehaviourThread::CompileBehaviourThread()
{
}

void CompileBehaviourThread::run()
{
    NONULL(m_behaviourHolder);

    String sourceFilepath = m_behaviourHolder->GetSourceFilepath();

    // Compile
    String soFilepath = SystemUtils::CompileToSharedObject(sourceFilepath);

    // Check, just in case the BehaviourHolder has been deleted while compiling
    if (m_behaviourHolder)
    {
        m_behaviourHolder->OnBehaviourFinishedCompiling(soFilepath); // Notify
    }

    deleteLater(); // Automatic delete
}

void CompileBehaviourThread::SetBehaviourHolder(BehaviourHolder *bh)
{
    m_behaviourHolder = bh;
}
