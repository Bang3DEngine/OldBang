#include "BehaviourHolder.h"

BehaviourHolder::BehaviourHolder()
{
    #ifdef BANG_EDITOR
    m_inspectorComponentInfo.SetSlotsInfos(
    {
        new InspectorFileSWInfo( "Behaviour", "cpp" ),
        new InspectorButtonSWInfo( "Refresh",
                    std::bind(&BehaviourHolder::Refresh, this) )
    }
    );
    #endif
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

void BehaviourHolder::Refresh()
{
    if (m_sourceFilepath == "") return;

    // Compile
    std::string soFilepath =
            SystemUtils::CompileToSharedObject(m_sourceFilepath);
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
        }
    }
    else
    {
        Logger_Error("Behaviour " << m_sourceFilepath <<
                     " could not be refreshed. See errors above");
    }
}


#ifdef BANG_EDITOR
InspectorWidgetInfo* BehaviourHolder::GetComponentInfo()
{
    static_cast<InspectorFileSWInfo*>
            (m_inspectorComponentInfo.GetSlotInfo(0))->filepath = m_sourceFilepath;

    return &m_inspectorComponentInfo;
}

void BehaviourHolder::OnSlotValueChanged(InspectorWidget *source)
{
    m_sourceFilepath = source->GetSWFileFilepath("Behaviour");
    Refresh();
}
#endif

void BehaviourHolder::Write(std::ostream &f) const
{
    f << "<BehaviourHolder>" << std::endl;
    f << ((void*)this) << std::endl;
    FileWriter::WriteFilepath(m_sourceFilepath, f);
    f << "</BehaviourHolder>" << std::endl;
}

void BehaviourHolder::Read(std::istream &f)
{
    FileReader::RegisterNextPointerId(f, this);
    m_sourceFilepath = FileReader::ReadString(f);
    FileReader::ReadNextLine(f); //Consume close tag
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

void BehaviourHolder::_OnDestroy()
{
    Component::_OnDestroy();
    if (m_behaviour)
    {
        m_behaviour->_OnDestroy();
    }
}
