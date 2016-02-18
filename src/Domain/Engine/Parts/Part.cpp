#include "Part.h"
#include "Entity.h"

Part::Part() : parent(nullptr)
{
}

Part::~Part()
{

}

Entity *Part::GetParent() const
{
    return parent;
}

const std::string Part::ToString() const
{
    return "Part";
}
