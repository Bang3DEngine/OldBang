#include "Bang/FontAssetFile.h"

#include "Bang/IO.h"
#include "Bang/Debug.h"
#include "Bang/FileReader.h"
#include "Bang/IconManager.h"
#include "Bang/AssetsManager.h"

#ifdef BANG_EDITOR
#include "Bang/SerializableObject.h"
#endif

FontAssetFile::FontAssetFile(const QFileSystemModel *model, const QModelIndex &index)
{
    XMLNode *xmlInfo;
    xmlInfo = XMLParser::FromFile(m_path);
    if (xmlInfo)
    {
        m_trueFontFilepath = xmlInfo->GetFilepath("FontFilepath");
        delete xmlInfo;
    }
}

const QPixmap& FontAssetFile::GetIcon() const
{
    String path = IO::ToAbsolute("./Icons/LetterIcon.png", true);
    return IconManager::LoadPixmap(path, IconManager::IconOverlay::Asset);
}

#ifdef BANG_EDITOR
SerializableObject *FontAssetFile::GetNewInspectable() const
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
