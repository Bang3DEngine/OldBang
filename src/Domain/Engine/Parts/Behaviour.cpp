#include "Behaviour.h"

Behaviour::Behaviour()
{
    #ifdef BANG_EDITOR
    inspectorPartInfo.slotInfos =
    {
        new InspectorPartInfoSlotAsset( "Script", "cpp" )
    };
    #endif
}

#ifdef BANG_EDITOR
InspectorPartInfo* Behaviour::GetPartInfo()
{
    static_cast<InspectorPartInfoSlotAsset*>
            (inspectorPartInfo.slotInfos[0])->filepath = filepath;

    return &inspectorPartInfo;
}

void Behaviour::OnInspectorSlotChanged(InspectorPartWidget *partWidget)
{
    filepath = partWidget->GetSWFileFilepath("Script");
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
            static_cast<InspectorPartInfoSlotAsset*>
            (inspectorPartInfo.slotInfos[0])->filepath;
    return "Behaviour ( " + file + ")";
}
