#include "BehaviourHolder.h"

BehaviourHolder::BehaviourHolder()
{
    #ifdef BANG_EDITOR
    inspectorComponentInfo.SetSlotsInfos(
    {
        new InspectorFileSWInfo( "Behaviour", "cpp" )
    });
    #endif
}

BehaviourHolder::~BehaviourHolder()
{
}

void BehaviourHolder::ChangeBehaviour(Behaviour *newBehaviour)
{
    if(behaviour != nullptr)
    {
        behaviour->behaviourHolder = nullptr;
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


#ifdef BANG_EDITOR
InspectorWidgetInfo* BehaviourHolder::GetComponentInfo()
{
    std::string bFilepath = "";
    if(behaviour != nullptr)
    {
        bFilepath = behaviour->GetSourceFilepath();
    }

    static_cast<InspectorFileSWInfo*>
            (inspectorComponentInfo.GetSlotInfo(0))->filepath = bFilepath;

    return &inspectorComponentInfo;
}

void BehaviourHolder::OnSlotValueChanged(InspectorWidget *source)
{
    std::string sourceFilepath = source->GetSWFileFilepath("Behaviour");

    std::string soFilepath = SystemUtils::CompileToSharedObject(sourceFilepath);
    if(soFilepath == "")
    {
        ChangeBehaviour(nullptr);
        return;
    }

    Behaviour *b = SystemUtils::CreateDynamicBehaviour(soFilepath);
    ChangeBehaviour(b);
    behaviour->SetSourceFilepath(sourceFilepath);
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
