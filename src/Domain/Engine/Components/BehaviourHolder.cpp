#include "BehaviourHolder.h"

BehaviourHolder::BehaviourHolder()
{
    #ifdef BANG_EDITOR
    inspectorComponentInfo.SetSlotsInfos(
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
    if(currentOpenLibrary != nullptr)
    {
        SystemUtils::CloseLibrary(currentOpenLibrary);
    }

    if(behaviour != nullptr)
    {
        delete behaviour;
    }
}

void BehaviourHolder::ChangeBehaviour(Behaviour *newBehaviour)
{
    if(behaviour != nullptr)
    {
        delete behaviour;
    }

    behaviour = newBehaviour;
    if(behaviour != nullptr)
    {
        behaviour->behaviourHolder = this;
    }
}


const std::string BehaviourHolder::ToString() const
{
    std::string file =
            static_cast<InspectorFileSWInfo*>
            (inspectorComponentInfo.GetSlotInfo(0))->filepath;
    return "BehaviourHolder ( " + file + ")";
}

void BehaviourHolder::Refresh()
{
    if(sourceFilepath == "") return;

    // Compile
    std::string soFilepath =
            SystemUtils::CompileToSharedObject(sourceFilepath);
    if(soFilepath == "")
    {
        ChangeBehaviour(nullptr);
        Logger_Error("There was an error compiling the Behaviour "
                     << sourceFilepath);
        return;
    }

    // Create new Behaviour
    void *openLibrary = nullptr;
    Behaviour *createdBehaviour = nullptr;
    SystemUtils::CreateDynamicBehaviour(soFilepath,
                                        &createdBehaviour,
                                        &openLibrary);

    if(createdBehaviour != nullptr)
    {
        if(currentOpenLibrary != nullptr)
        {
            SystemUtils::CloseLibrary(currentOpenLibrary);
        }
        currentOpenLibrary = openLibrary;
    }

    Logger_Log("Old behaviour: " << behaviour);
    ChangeBehaviour(createdBehaviour); // To newly created or nullptr, depending on success
    Logger_Log("New behaviour: " << (void*)behaviour);

    if(createdBehaviour != nullptr)
    {
        if(behaviour != nullptr)
        {
            behaviour->SetSourceFilepath(sourceFilepath);
            behaviour->_OnStart();
        }
    }
    else
    {
        Logger_Error("Behaviour " << sourceFilepath <<
                     " could not be refreshed. See errors above");
    }
    Logger_Log("created");
}


#ifdef BANG_EDITOR
InspectorWidgetInfo* BehaviourHolder::GetComponentInfo()
{
    static_cast<InspectorFileSWInfo*>
            (inspectorComponentInfo.GetSlotInfo(0))->filepath = sourceFilepath;

    return &inspectorComponentInfo;
}

void BehaviourHolder::OnSlotValueChanged(InspectorWidget *source)
{
    sourceFilepath = source->GetSWFileFilepath("Behaviour");
}

void BehaviourHolder::Write(std::ostream &f) const
{
    /*
    f << "<BehaviourHolder>" << std::endl;
    f << ((void*)this) << std::endl;
    f << "</BehaviourHolder>" << std::endl;
    */
}

void BehaviourHolder::Read(std::istream &f)
{

}

#endif

void BehaviourHolder::_OnStart()
{
    Component::_OnStart();
    if(behaviour != nullptr)
    {
        behaviour->_OnStart();
    }
}

void BehaviourHolder::_OnUpdate()
{
    Component::_OnUpdate();
    if(behaviour != nullptr)
    {
        behaviour->_OnUpdate();
    }
}

void BehaviourHolder::_OnPreRender()
{
    Component::_OnPreRender();
    if(behaviour != nullptr)
    {
        behaviour->_OnPreRender();
    }
}

void BehaviourHolder::_OnRender()
{
    Component::_OnRender();
    if(behaviour != nullptr)
    {
        behaviour->_OnRender();
    }
}

void BehaviourHolder::_OnDestroy()
{
    Component::_OnDestroy();
    if(behaviour != nullptr)
    {
        behaviour->_OnDestroy();
    }
}
