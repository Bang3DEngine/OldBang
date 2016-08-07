#include "BehaviourHolder.h"

BehaviourHolder::BehaviourHolder()
{
    //    new AttrWidgetButtonInfo( "Refresh",
    //                std::bind(&BehaviourHolder::Refresh, this) )

    m_compileThread.SetBehaviourHolder(this);
}

BehaviourHolder::~BehaviourHolder()
{
    if (m_behaviour)
    {
        delete m_behaviour;
    }

    if (m_currentOpenLibrary)
    {
        SystemUtils::CloseLibrary(m_currentOpenLibrary);
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


const std::string BehaviourHolder::ToString() const
{
    return "BehaviourHolder ( " + m_sourceFilepath + ")";
}

std::string BehaviourHolder::GetName() const
{
    return "BehaviourHolder";
}

void BehaviourHolder::CloneInto(ICloneable *clone) const
{
    Component::CloneInto(clone);
    BehaviourHolder *bh = static_cast<BehaviourHolder*>(clone);
    bh->m_sourceFilepath = m_sourceFilepath;
    bh->Refresh();
}

ICloneable *BehaviourHolder::Clone() const
{
    BehaviourHolder *bh = new BehaviourHolder();
    CloneInto(bh);
    return bh;
}

const std::string &BehaviourHolder::GetSourceFilepath() const
{
    return m_sourceFilepath;
}

void BehaviourHolder::Refresh()
{
    if (m_sourceFilepath == "") return;

    if (!m_compileThread.isRunning())
    {
        std::string filename = Persistence::GetFileNameWithExtension(m_sourceFilepath);
        Logger_Log("Refreshing Behaviour '" << filename << "'...");
        m_compileThread.start();
    }
}


void BehaviourHolder::OnBehaviourFinishedCompiling(std::string soFilepath)
{
    if (soFilepath == "")
    {
        ChangeBehaviour(nullptr);
        Logger_Error("There was an error compiling the Behaviour "
                     << m_sourceFilepath);
        return;
    }

    // Create new Behaviour
    void *openLibrary = nullptr;
    Behaviour *createdBehaviour = nullptr;
    SystemUtils::CreateDynamicBehaviour(soFilepath,
                                        &createdBehaviour,
                                        &openLibrary);

    ChangeBehaviour(createdBehaviour); // To newly created or nullptr, depending on success

    if (createdBehaviour)
    {
        if (m_currentOpenLibrary)
        {
            SystemUtils::CloseLibrary(m_currentOpenLibrary);
        }
        m_currentOpenLibrary = openLibrary;
    }

    if (createdBehaviour)
    {
        if (m_behaviour)
        {
            m_behaviour->Init(this);
            m_behaviour->_OnStart();
            std::string filename = Persistence::GetFileNameWithExtension(m_sourceFilepath);
            Logger_Log("Behaviour '" << filename << "' successfully refreshed!");
        }
    }
    else
    {
        Logger_Error("Behaviour " << m_sourceFilepath <<
                     " could not be refreshed. See errors above");
    }
}

void BehaviourHolder::OnButtonClicked()
{
    Refresh();
}

void BehaviourHolder::ReadXMLInfo(const XMLNode *xmlInfo)
{
    Component::ReadXMLInfo(xmlInfo);
    std::string lastFilepath = m_sourceFilepath;
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
    if (m_compileThread.isRunning())
    {
        xmlInfo->SetButton("Refresh", noConstThis, {XMLProperty::Disabled});
    }
    else
    {
        xmlInfo->SetButton("Refresh", noConstThis, {});
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

void BehaviourHolder::_OnDestroy()
{
    Component::_OnDestroy();
    if (m_behaviour)
    {
        m_behaviour->_OnDestroy();
    }
}


void CompileBehaviourThread::run()
{
    std::string sourceFilepath = m_behaviourHolder->GetSourceFilepath();
    std::string soFilepath = SystemUtils::CompileToSharedObject(sourceFilepath);

    // Here it's compiling the behaviour...
    m_behaviourHolder->OnBehaviourFinishedCompiling(soFilepath); // Notify
}

void CompileBehaviourThread::SetBehaviourHolder(BehaviourHolder *bh)
{
    m_behaviourHolder = bh;
}
