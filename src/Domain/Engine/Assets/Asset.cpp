#include "Asset.h"

Asset::Asset()
{

}

Asset::~Asset()
{

}

std::string Asset::GetFilepath() const
{
    return m_filepath;
}

const std::string Asset::ToString() const
{
    return "Asset '" + m_filepath + "'.";
}

std::string Asset::GetTag() const
{
    return "Asset";
}

void Asset::WriteInternal(std::ostream &f) const
{
}

void Asset::ReadInternal(std::istream &f)
{
}
