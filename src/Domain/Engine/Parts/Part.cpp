#include "Part.h"
#include "Entity.h"

Part::Part() : owner(nullptr)
{
}

Part::~Part()
{
#ifdef BANG_EDITOR
    for(int i = 0; i < inspectorPartInfo.slotInfos.size(); ++i)
    {
        //The type of T determines the delete we have to do (we can't delete a void*)
        InspectorPartSlotInfo<float> *pfloat =
                dynamic_cast< InspectorPartSlotInfo<float>* >(inspectorPartInfo.slotInfos[i]);
        if(pfloat != nullptr) { delete pfloat; continue; }
/*
        InspectorPartSlotInfo< std::vector<float> > *pvfloat =
                dynamic_cast<InspectorPartSlotInfo< std::vector<float> >*>(inspectorPartInfo.slotInfos[i]);
        if(pvfloat != nullptr) { delete pvfloat; continue; }

        InspectorPartSlotInfo< std::vector<std::string> > *penum =
                dynamic_cast<InspectorPartSlotInfo< std::vector<std::string> >*>(inspectorPartInfo.slotInfos[i]);
        if(penum != nullptr) { delete penum; continue; }
*/
        Logger_Error("THIS SHOULD NEVER HAPPEN, ADD THE CORRESPONDING DELETE!");
    }
#endif
}

Entity *Part::GetOwner() const
{
    return owner;
}

const std::string Part::ToString() const
{
    std::ostringstream msg;
    msg << "Part";
    return msg.str();
}

#ifdef BANG_EDITOR
InspectorPartInfo* Part::GetInfo()
{
    return &inspectorPartInfo;
}
#endif
