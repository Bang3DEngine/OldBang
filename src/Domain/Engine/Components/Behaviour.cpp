#include "Behaviour.h"

Behaviour::Behaviour()
{
    #ifdef BANG_EDITOR
    inspectorComponentInfo.SetSlotsInfos(
    {
        new InspectorFileSWInfo( "Script", "cpp" )
    });
    #endif
}

#ifdef BANG_EDITOR
InspectorWidgetInfo* Behaviour::GetComponentInfo()
{
    static_cast<InspectorFileSWInfo*>
            (inspectorComponentInfo.GetSlotInfo(0))->filepath = filepath;

    return &inspectorComponentInfo;
}

void Behaviour::OnSlotValueChanged(InspectorWidget *source)
{
    filepath = source->GetSWFileFilepath("Script");
}

void Behaviour::Write(std::ostream &f) const
{
    /*
    f << "<Behaviour>" << std::endl;
    f << ((void*)this) << std::endl;
    f << "</Behaviour>" << std::endl;
    */
}

void Behaviour::Read(std::istream &f)
{

}

void Behaviour::SetFilepath(const std::string &filepath)
{
    this->filepath = filepath;
}

std::string Behaviour::GetFilepath() const
{
    return filepath;
}

#endif

const std::string Behaviour::ToString() const
{
    std::string file =
            static_cast<InspectorFileSWInfo*>
            (inspectorComponentInfo.GetSlotInfo(0))->filepath;
    return "Behaviour ( " + file + ")";
}
