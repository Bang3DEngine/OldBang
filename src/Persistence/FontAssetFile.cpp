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
    String fp = Persistence::ToAbsolute("./Assets/Engine/Icons/NoAssetIcon.png");
    QPixmap pm(QString::fromStdString(fp));
    return pm;
}

#ifdef BANG_EDITOR
IInspectable *FontAssetFile::GetInspectable() const
{
    return nullptr;
}
#endif

const String &FontAssetFile::GetFontFilepath() const
{
    return m_trueFontFilepath;
}
