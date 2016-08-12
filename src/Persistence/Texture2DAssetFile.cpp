#include "Texture2DAssetFile.h"

#include "XMLNode.h"
#include "FileReader.h"
#include "Texture2DAssetFileInspectable.h"

Texture2DAssetFile::Texture2DAssetFile
    (const QFileSystemModel *model, const QModelIndex &index) : File(model, index)
{
}

std::string Texture2DAssetFile::GetImageFilepath() const
{
    std::string contents = FileReader::GetContents(GetAbsolutePath());
    XMLNode *xmlInfo = XMLNode::FromString(contents);
    std::string result = "";
    if (xmlInfo)
    {
        result = xmlInfo->GetString("ImageFilepath");
        delete xmlInfo;
    }
    return result;
}

QPixmap Texture2DAssetFile::GetIcon() const
{
    std::string fp = GetImageFilepath();
    QPixmap pm(QString::fromStdString(fp));
    return pm;
}

IInspectable *Texture2DAssetFile::GetInspectable() const
{
    return new Texture2DAssetFileInspectable(*this);
}
