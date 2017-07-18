#include "Bang/MaterialFile.h"

#include "Bang/Paths.h"
#include "Bang/Material.h"
#include "Bang/XMLParser.h"
#include "Bang/AssetsManager.h"

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

void MaterialFile::Read(const XMLNode &xmlInfo)
{
    BFile::Read(xmlInfo);

    m_xmlInfo = xmlInfo;
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
}


bool MaterialFile::IsAsset() const
{
    return true;
}
