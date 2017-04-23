#include "ShaderProgramAssetFileInspectable.h"

#include "Bang/IO.h"
#include "Bang/ShaderProgram.h"
#include "Bang/AssetsManager.h"

ShaderProgramAssetFileInspectable::ShaderProgramAssetFileInspectable(
        const ShaderProgramAssetFile &spFile)
    : m_shaderProgramFile(spFile)
{

}

void ShaderProgramAssetFileInspectable::Read(const XMLNode &xmlInfo)
{
    SerializableObject::Read(xmlInfo);
    String shaderProgramFilepath = m_shaderProgramFile.GetAbsolutePath();
    IO::WriteToFile(shaderProgramFilepath, xmlInfo.ToString());
    AssetsManager::UpdateAsset(shaderProgramFilepath, xmlInfo);
}

void ShaderProgramAssetFileInspectable::Write(XMLNode *xmlInfo) const
{
    SerializableObject::Write(xmlInfo);
    xmlInfo->SetTagName("ShaderProgram");
    xmlInfo->SetFilepath("VertexShader",
                         m_shaderProgramFile.GetVertexShaderFilepath(),
                         "vert glsl");
    xmlInfo->SetFilepath("FragmentShader",
                         m_shaderProgramFile.GetFragmentShaderFilepath(),
                         "frag glsl");
}
