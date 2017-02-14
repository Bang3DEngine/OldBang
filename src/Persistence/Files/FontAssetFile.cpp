#include "FontAssetFile.h"

#include "Debug.h"
#include "FileReader.h"
#include "Persistence.h"
#include "AssetsManager.h"

#ifdef BANG_EDITOR
#include "IInspectable.h"
#endif

FontAssetFile::FontAssetFile(const QFileSystemModel *model, const QModelIndex &index)
{
    XMLNode *xmlInfo;
    xmlInfo = XMLParser::FromFile(m_path);
    m_trueFontFilepath = xmlInfo->GetFilepath("FontFilepath");
    delete xmlInfo;
}

QPixmap FontAssetFile::GetIcon() const
{
    String fp = Persistence::ToAbsolute("./Icons/NoAssetIcon.png", true);
    QPixmap pm(fp.ToQString());
    return pm;
}

#ifdef BANG_EDITOR
IInspectable *FontAssetFile::GetInspectable() const
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
