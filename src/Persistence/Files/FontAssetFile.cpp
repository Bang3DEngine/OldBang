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
    XMLNode xmlInfo = XMLParser::FromFile(GetPath().GetAbsolute());
    m_trueFontFilepath = xmlInfo.GetFilepath("FontFilepath").GetAbsolute();
}

const QPixmap& FontAssetFile::GetIcon() const
{
    return IconManager::LoadPixmap(EPATH("Icons/LetterIcon.png"),
                                   IconManager::IconOverlay::Asset);
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
