#include "Bang/MaterialFile.h"

#include "Bang/Paths.h"
#include "Bang/Material.h"
#include "Bang/XMLParser.h"
#include "Bang/IconManager.h"
#include "Bang/AssetsManager.h"
#include "Bang/FileInspectable.h"

MaterialFile::MaterialFile()
{
}

MaterialFile::MaterialFile(const Path& path) :
    BFile(path)
{
    XMLNode xmlInfo = XMLParser::FromFile( GetPath() );
    m_vshaderFilepath = xmlInfo.GetFilepath("VertexShader");
    m_fshaderFilepath = xmlInfo.GetFilepath("FragmentShader");
    m_textureFilepaths.PushBack( xmlInfo.GetFilepath("Texture") );
    m_diffuseColor = xmlInfo.GetColor("DiffuseColor");

    XMLNode xmlMatInfo = XMLParser::FromFile( GetPath() );
    m_xmlInfo = xmlMatInfo;
}

const QPixmap& MaterialFile::GetIcon() const
{
    Path absPath = GetPath();
    Material *mat = AssetsManager::Load<Material>(absPath);
    return IconManager::LoadMaterialPixmap(mat);
}


void MaterialFile::Read(const XMLNode &xmlInfo)
{
    BFile::Read(xmlInfo);

    m_xmlInfo = xmlInfo;

    #ifdef BANG_EDITOR
    // Update live instances currently being used
    AssetsManager::UpdateAsset(GetPath(), xmlInfo);
    #endif
}

void MaterialFile::Write(XMLNode *xmlInfo) const
{
    BFile::Write(xmlInfo);

    // Do Read & Write so that old files with different formats have the
    // newer one
    Material mat;
    mat.Read(m_xmlInfo);
    mat.Write(xmlInfo);

    m_xmlInfo = *xmlInfo;

    IconManager::InvalidatePixmap( GetPath() );
}


#ifdef BANG_EDITOR
IInspectable *MaterialFile::GetNewInspectable()
{
    return new FileInspectable<MaterialFile>(*this);
}
#endif

bool MaterialFile::IsAsset() const
{
    return true;
}
