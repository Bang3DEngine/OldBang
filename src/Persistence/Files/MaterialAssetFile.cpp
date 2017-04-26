#include "Bang/MaterialAssetFile.h"

#include "Bang/IO.h"
#include "Bang/Material.h"
#include "Bang/XMLParser.h"
#include "Bang/IconManager.h"
#include "Bang/AssetsManager.h"
#include "Bang/FileInspectable.h"

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

    XMLNode *xmlMatInfo = XMLParser::FromFile( GetRelativePath() );
    if (xmlMatInfo)
    {
        m_xmlInfo = *xmlMatInfo;
        delete xmlMatInfo;
    }
}

const QPixmap& MaterialAssetFile::GetIcon() const
{
    String absPath = GetAbsolutePath();
    Material *mat = AssetsManager::Load<Material>(absPath);
    return IconManager::LoadMaterialPixmap(mat);
}


void MaterialAssetFile::Read(const XMLNode &xmlInfo)
{
    // Update live instances currently being used
    AssetsManager::UpdateAsset(GetAbsolutePath(), xmlInfo);
    m_xmlInfo = xmlInfo;
}

void MaterialAssetFile::Write(XMLNode *xmlInfo) const
{
    File::Write(xmlInfo);

    // Do Read & Write so that old files with different formats have the
    // newer one
    Material *mat = new Material();
    mat->Read(m_xmlInfo);
    mat->Write(xmlInfo);

    m_xmlInfo = *xmlInfo;

    delete mat;

    IconManager::InvalidatePixmap( GetAbsolutePath() );
}


#ifdef BANG_EDITOR
IInspectable *MaterialAssetFile::GetNewInspectable()
{
    return new FileInspectable<MaterialAssetFile>(*this);
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
