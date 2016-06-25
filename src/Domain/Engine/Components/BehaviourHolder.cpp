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
    if(behaviour )
    {
        delete behaviour;
    }

    if(currentOpenLibrary )
    {
        SystemUtils::CloseLibrary(currentOpenLibrary);
    }
}

void BehaviourHolder::ChangeBehaviour(Behaviour *newBehaviour)
{
    if(behaviour )
    {
        delete behaviour;
    }

    behaviour = newBehaviour;
    if(behaviour )
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

    ChangeBehaviour(createdBehaviour); // To newly created or nullptr, depending on success

    if(createdBehaviour )
    {
        if(currentOpenLibrary )
        {
            SystemUtils::CloseLibrary(currentOpenLibrary);
        }
        currentOpenLibrary = openLibrary;
    }

    if(createdBehaviour )
    {
        if(behaviour )
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
    Refresh();
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
    if(behaviour )
    {
        behaviour->_OnStart();
    }
}

void BehaviourHolder::_OnUpdate()
{
    Component::_OnUpdate();
    if(behaviour )
    {
        behaviour->_OnUpdate();
    }
}

void BehaviourHolder::_OnPreRender()
{
    Component::_OnPreRender();
    if(behaviour )
    {
        behaviour->_OnPreRender();
    }
}

void BehaviourHolder::_OnRender()
{
    Component::_OnRender();
    if(behaviour )
    {
        behaviour->_OnRender();
    }
}

void BehaviourHolder::_OnDestroy()
{
    Component::_OnDestroy();
    if(behaviour )
    {
        behaviour->_OnDestroy();
    }
}
