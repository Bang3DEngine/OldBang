#include "FontAssetFile.h"

#include "IO.h"
#include "Debug.h"
#include "FileReader.h"
#include "IconManager.h"
#include "AssetsManager.h"

#ifdef BANG_EDITOR
#include "SerializableObject.h"
#endif

FontAssetFile::FontAssetFile(const QFileSystemModel *model, const QModelIndex &index)
{
    XMLNode *xmlInfo;
    xmlInfo = XMLParser::FromFile(m_path);
    m_trueFontFilepath = xmlInfo->GetFilepath("FontFilepath");
    delete xmlInfo;
}

const QPixmap& FontAssetFile::GetIcon() const
{
    String path = IO::ToAbsolute("./Icons/LetterIcon.png", true);
    return IconManager::LoadPixmap(path, IconManager::IconOverlay::Asset);
}

#ifdef BANG_EDITOR
SerializableObject *FontAssetFile::GetInspectable() const
{
    return nullptr;
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
