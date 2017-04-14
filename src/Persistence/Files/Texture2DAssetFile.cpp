#include "Bang/Texture2DAssetFile.h"

#include "Bang/IO.h"
#include "Bang/Debug.h"
#include "Bang/XMLNode.h"
#include "Bang/FileReader.h"
#include "Bang/IconManager.h"

#ifdef BANG_EDITOR
#include "Bang/SerializableObject.h"
#include "Bang/Texture2DAssetFileInspectable.h"
#endif

Texture2DAssetFile::Texture2DAssetFile()
{
}

Texture2DAssetFile::Texture2DAssetFile
    (const QFileSystemModel *model, const QModelIndex &index) : File(model, index)
{
}

String Texture2DAssetFile::GetImageAbsFilepath() const
{
    String contents = IO::GetFileContents(GetAbsolutePath());
    XMLNode *xmlInfo = XMLNode::FromString(contents);
    String result = "";
    if (xmlInfo)
    {
        result = xmlInfo->GetFilepath("ImageFilepath");
        delete xmlInfo;
    }
    return result;
}

const QPixmap& Texture2DAssetFile::GetIcon() const
{
    return IconManager::LoadPixmap(GetImageAbsFilepath(),
                                   IconManager::IconOverlay::Asset);
}

#ifdef BANG_EDITOR
SerializableObject *Texture2DAssetFile::GetInspectable() const
{
    return new Texture2DAssetFileInspectable(*this);
}
#endif

bool Texture2DAssetFile::IsAsset() const
{
    return true;
}
