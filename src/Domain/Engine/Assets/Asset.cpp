#include "Asset.h"

Asset::Asset()
{

}

Asset::~Asset()
{

}

std::string Asset::GetFilepath() const
{
    return filepath;
}

const std::string Asset::ToString() const
{
    return "Asset '" + filepath + "'.";
}

void Asset::Write(std::ostream &f) const
{
}

void Asset::Read(std::istream &f)
{
}
