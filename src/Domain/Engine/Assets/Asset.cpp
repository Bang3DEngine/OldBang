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

void Asset::Write(std::ostream &f) const
{
}

void Asset::Read(std::istream &f)
{
}
