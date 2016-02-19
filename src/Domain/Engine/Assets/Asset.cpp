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


void Asset::Write(std::ofstream &f) const
{
}

void Asset::Read(std::ifstream &f)
{
}