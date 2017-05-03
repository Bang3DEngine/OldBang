#include "Bang/FontFile.h"

#include "Bang/Paths.h"
#include "Bang/Debug.h"
#include "Bang/FileReader.h"
#include "Bang/IconManager.h"
#include "Bang/AssetsManager.h"
#include "Bang/FileInspectable.h"

FontFile::FontFile()
{
}

FontFile::FontFile(const Path& path) : File(path)
{
    XMLNode xmlInfo = XMLParser::FromFile( GetPath() );
    m_trueFontFilepath = xmlInfo.GetFilepath("FontFilepath");
}

const QPixmap& FontFile::GetIcon() const
{
    return IconManager::LoadPixmap(EPATH("Icons/LetterIcon.png"),
                                   IconManager::IconOverlay::Asset);
}

#ifdef BANG_EDITOR
IInspectable *FontFile::GetNewInspectable()
{
    return new FileInspectable<FontFile>(*this);
}
#endif

bool FontFile::IsAsset() const
{
    return true;
}

const Path &FontFile::GetFontFilepath() const
{
    return m_trueFontFilepath;
}
