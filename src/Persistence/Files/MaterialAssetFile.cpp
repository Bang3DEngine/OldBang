#include "Bang/MaterialAssetFile.h"

#include "Bang/IO.h"
#include "Bang/Material.h"
#include "Bang/XMLParser.h"
#include "Bang/IconManager.h"
#include "Bang/AssetsManager.h"

#ifdef BANG_EDITOR
#include "Bang/MaterialAssetFileInspectable.h"
#endif

MaterialAssetFile::MaterialAssetFile()
{
}

MaterialAssetFile::MaterialAssetFile(const QFileSystemModel *model,
                                     const QModelIndex &index) :
    File(model, index)
{
    XMLNode *xmlInfo;
    xmlInfo = XMLParser::FromFile(m_path);
    if (xmlInfo)
    {
        m_vshaderFilepath = xmlInfo->GetString("VertexShader");
        m_fshaderFilepath = xmlInfo->GetString("FragmentShader");

        m_textureFilepaths.PushBack( xmlInfo->GetFilepath("Texture") );

        m_diffuseColor = xmlInfo->GetColor("DiffuseColor");
        delete xmlInfo;
    }
}

const QPixmap& MaterialAssetFile::GetIcon() const
{
    String absPath = GetAbsolutePath();
    Material *mat = AssetsManager::Load<Material>(absPath);
    return IconManager::LoadMaterialPixmap(mat);
}

#ifdef BANG_EDITOR
IInspectable *MaterialAssetFile::GetNewInspectable() const
{
    return nullptr;//new MaterialAssetFileInspectable(*this);
}
#endif

bool MaterialAssetFile::IsAsset() const
{
    return true;
}

const String &MaterialAssetFile::GetVertexShaderFilepath() const
{
    return m_vshaderFilepath;
}

const String &MaterialAssetFile::GetFragmentShaderFilepath() const
{
    return m_fshaderFilepath;
}

const Array<String> &MaterialAssetFile::GetTextureFilepaths() const
{
    return m_textureFilepaths;
}

const Color &MaterialAssetFile::GetDiffuseColor() const
{
    return m_diffuseColor;
}
