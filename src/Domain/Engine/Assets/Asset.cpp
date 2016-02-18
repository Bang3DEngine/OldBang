#include "Asset.h"

Asset::Asset() : filepath(__FILE__)
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
