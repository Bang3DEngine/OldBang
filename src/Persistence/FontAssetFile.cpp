#include "FontAssetFile.h"

#include "FileReader.h"
#include "AssetsManager.h"

FontAssetFile::FontAssetFile(const QFileSystemModel *model, const QModelIndex &index)
{
    XMLNode *xmlInfo;
    xmlInfo = XMLParser::FromFile(m_path);
    m_trueFontFilepath = xmlInfo->GetFilepath("FontFilepath");
    delete xmlInfo;
}

QPixmap FontAssetFile::GetIcon() const
{
    String fp = Persistence::ToAbsolute("./Assets/Engine/Icons/NoAssetIcon.png");
    QPixmap pm(QString::fromStdString(fp));
    return pm;
}

IInspectable *FontAssetFile::GetInspectable() const
{
    return nullptr;
}

const String &FontAssetFile::GetFontFilepath() const
{
    return m_trueFontFilepath;
}
