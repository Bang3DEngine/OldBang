#include "Bang/FontAssetFile.h"

#include "Bang/IO.h"
#include "Bang/Debug.h"
#include "Bang/FileReader.h"
#include "Bang/IconManager.h"
#include "Bang/AssetsManager.h"
#include "Bang/FileInspectable.h"

FontAssetFile::FontAssetFile()
{
}

FontAssetFile::FontAssetFile(const QFileSystemModel *model, const QModelIndex &index)
{
    XMLNode xmlInfo = XMLParser::FromFile(m_path);
    m_trueFontFilepath = xmlInfo.GetFilepath("FontFilepath");
}

const QPixmap& FontAssetFile::GetIcon() const
{
    String path = IO::ToAbsolute("./Icons/LetterIcon.png", true);
    return IconManager::LoadPixmap(path, IconManager::IconOverlay::Asset);
}

#ifdef BANG_EDITOR
IInspectable *FontAssetFile::GetNewInspectable()
{
    return new FileInspectable<FontAssetFile>(*this);
}
#endif

bool FontAssetFile::IsAsset() const
{
    return true;
}

const String &FontAssetFile::GetFontFilepath() const
{
    return m_trueFontFilepath;
}
