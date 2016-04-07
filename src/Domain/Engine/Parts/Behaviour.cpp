#include "Behaviour.h"

Behaviour::Behaviour()
{
    #ifdef BANG_EDITOR
    inspectorPartInfo.SetSlotsInfos(
    {
        new InspectorWidgetInfoSlotAsset( "Script", "cpp" )
    });
    #endif
}

#ifdef BANG_EDITOR
InspectorWidgetInfo* Behaviour::GetPartInfo()
{
    static_cast<InspectorWidgetInfoSlotAsset*>
            (inspectorPartInfo.GetSlotInfo(0))->filepath = filepath;

    return &inspectorPartInfo;
}

void Behaviour::OnSlotValueChanged(InspectorWidget *source)
{
    filepath = source->GetSWFileFilepath("Script");
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
            static_cast<InspectorWidgetInfoSlotAsset*>
            (inspectorPartInfo.GetSlotInfo(0))->filepath;
    return "Behaviour ( " + file + ")";
}
