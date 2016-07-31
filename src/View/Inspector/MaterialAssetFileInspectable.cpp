#include "MaterialAssetFileInspectable.h"

MaterialAssetFileInspectable::MaterialAssetFileInspectable
    (const MaterialAssetFile &materialAssetFile) :
        m_materialAssetFile(materialAssetFile)
{

}

void MaterialAssetFileInspectable::OnInspectorXMLChanged(const XMLNode *xmlInfo)
{

}

void MaterialAssetFileInspectable::OnInspectorXMLNeeded(XMLNode *xmlInfo) const
{
    xmlInfo->SetFilepath("VertexShader", m_materialAssetFile.GetVertexShaderFilepath());
    xmlInfo->SetFilepath("FragmentShader", m_materialAssetFile.GetFragmentShaderFilepath());
    std::vector<std::string> textureFilepaths = m_materialAssetFile.GetTextureFilepaths();
    for (int i = 1; i <= textureFilepaths.size(); ++i)
    {
        xmlInfo->SetFilepath("Texture " + std::to_string(i), textureFilepaths[i-1]);
    }
    xmlInfo->SetVector4("DiffuseColor", m_materialAssetFile.GetDiffuseColor());
}
