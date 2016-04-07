#include "Part.h"
#include "Entity.h"


Part::Part() : owner(nullptr)
{
}

Part::~Part()
{
    for(int i = 0; i < inspectorPartInfo.GetSlotInfos().size(); ++i)
        delete inspectorPartInfo.GetSlotInfo(i);
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
