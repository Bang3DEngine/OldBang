#include "ShaderProgram.h"

#include "Bang/XMLNode.h"
#include "Bang/G_ShaderManager.h"

ShaderProgram::ShaderProgram()
{

}

String ShaderProgram::GetFileExtensionStatic()
{
    return "bshaderprogram";
}

String ShaderProgram::GetFileExtension() const
{
    return ShaderProgram::GetFileExtensionStatic();
}


String ShaderProgram::ToString() const
{
    std::ostringstream oss;
    oss << "Shader program: " << std::endl <<
           "   " << p_vshader << std::endl <<
           "   " << p_fshader << std::endl;
    return oss.str();
}

void ShaderProgram::Read(const XMLNode &xmlInfo)
{
    Asset::Read(xmlInfo);

    Path vShaderFilepath = xmlInfo.GetFilepath("VertexShader");
    if (vShaderFilepath.Exists())
    {
        G_Shader *vShader = G_ShaderManager::Load(G_Shader::Type::Vertex,
                                              vShaderFilepath);
        SetVertexShader(vShader);
    }

    Path fShaderFilepath = xmlInfo.GetFilepath("FragmentShader");
    if (fShaderFilepath.Exists())
    {
        G_Shader *fShader = G_ShaderManager::Load(G_Shader::Type::Fragment,
                                              fShaderFilepath);
        SetFragmentShader(fShader);
    }
}

void ShaderProgram::Write(XMLNode *xmlInfo) const
{
    Asset::Write(xmlInfo);

    Path vShaderFilepath = p_vshader ? p_vshader->GetFilepath() : Path();
    xmlInfo->SetFilepath("VertexShader",   vShaderFilepath, "vert glsl");

    Path fShaderFilepath = p_fshader ? p_fshader->GetFilepath() : Path();
    xmlInfo->SetFilepath("FragmentShader", fShaderFilepath, "frag glsl");
}
