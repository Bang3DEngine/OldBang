#include "Bang/FontFile.h"

#include "Bang/Paths.h"
#include "Bang/Debug.h"
#include "Bang/FileReader.h"
#include "Bang/AssetsManager.h"

FontFile::FontFile()
{
}

FontFile::FontFile(const Path& path) : BFile(path)
{
    XMLNode xmlInfo = XMLParser::FromFile( GetPath() );
    m_trueFontFilepath = xmlInfo.GetFilepath("FontFilepath");
}

bool FontFile::IsAsset() const
{
    return true;
}

const Path &FontFile::GetFontFilepath() const
{
    return m_trueFontFilepath;
}
