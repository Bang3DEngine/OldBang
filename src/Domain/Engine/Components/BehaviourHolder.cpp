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
    if (p_behaviour)
    {
        delete p_behaviour;
    }

    if (p_currentOpenLibrary)
    {
        SystemUtils::CloseLibrary(p_currentOpenLibrary);
    }
}

void BehaviourHolder::ChangeBehaviour(Behaviour *newBehaviour)
{
    if (p_behaviour)
    {
        delete p_behaviour;
    }

    p_behaviour = newBehaviour;
}


const std::string BehaviourHolder::ToString() const
{
    std::string file =
            static_cast<InspectorFileSWInfo*>
            (m_inspectorComponentInfo.GetSlotInfo(0))->filepath;
    return "BehaviourHolder ( " + file + ")";
}

ICloneable *BehaviourHolder::Clone() const
{
    return new BehaviourHolder();
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
        if (p_currentOpenLibrary)
        {
            SystemUtils::CloseLibrary(p_currentOpenLibrary);
        }
        p_currentOpenLibrary = openLibrary;
    }

    if (createdBehaviour)
    {
        if (p_behaviour)
        {
            p_behaviour->Init(this);
            p_behaviour->_OnStart();
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

#endif

void BehaviourHolder::_OnStart()
{
    Component::_OnStart();
    Refresh();
}

void BehaviourHolder::_OnUpdate()
{
    Component::_OnUpdate();
    if (p_behaviour)
    {
        p_behaviour->_OnUpdate();
    }
}

void BehaviourHolder::_OnPreRender()
{
    Component::_OnPreRender();
    if (p_behaviour)
    {
        p_behaviour->_OnPreRender();
    }
}

void BehaviourHolder::_OnRender()
{
    Component::_OnRender();
    if (p_behaviour)
    {
        p_behaviour->_OnRender();
    }
}

void BehaviourHolder::_OnDestroy()
{
    Component::_OnDestroy();
    if (p_behaviour)
    {
        p_behaviour->_OnDestroy();
    }
}
