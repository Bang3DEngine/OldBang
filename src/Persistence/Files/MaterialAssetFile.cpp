#include "MaterialAssetFile.h"

#include "IO.h"
#include "Material.h"
#include "XMLParser.h"
#include "IconManager.h"
#include "AssetsManager.h"

#ifdef BANG_EDITOR
#include "MaterialAssetFileInspectable.h"
#endif

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
    Material *mat = AssetsManager::Load<Material>( GetAbsolutePath() );
    return IconManager::LoadMaterialPixmap(mat);
}

#ifdef BANG_EDITOR
SerializableObject *MaterialAssetFile::GetInspectable() const
{
    return new MaterialAssetFileInspectable(*this);
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
