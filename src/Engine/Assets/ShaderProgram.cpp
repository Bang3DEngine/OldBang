#include "Bang/ShaderProgram.h"

#include "Bang/XMLNode.h"
#include "Bang/G_Shader.h"
#include "Bang/ShaderManager.h"

ShaderProgram::ShaderProgram() : G_ShaderProgram()
{

}

ShaderProgram::~ShaderProgram()
{
    if (p_vshader) { ShaderManager::UnRegisterUsageOfShader(this, p_vshader); }
    if (p_fshader) { ShaderManager::UnRegisterUsageOfShader(this, p_fshader); }
}

void ShaderProgram::Load(const Path &vshaderPath, const Path &fshaderPath)
{
    RetrieveType(fshaderPath);

    G_Shader *vs = ShaderManager::Load(G_Shader::Type::Vertex, vshaderPath);
    SetVertexShader(vs);

    G_Shader *fs = ShaderManager::Load(G_Shader::Type::Fragment, fshaderPath);
    SetFragmentShader(fs);
}

void ShaderProgram::SetVertexShader(G_Shader *vertexShader)
{
    if (p_vshader) { ShaderManager::UnRegisterUsageOfShader(this, p_vshader); }
    G_ShaderProgram::SetVertexShader(vertexShader);
    if (p_vshader) { ShaderManager::RegisterUsageOfShader(this, p_vshader); }
}

void ShaderProgram::SetFragmentShader(G_Shader *fragmentShader)
{
    if (p_fshader) { ShaderManager::UnRegisterUsageOfShader(this, p_fshader); }
    G_ShaderProgram::SetFragmentShader(fragmentShader);
    if (p_fshader) { ShaderManager::RegisterUsageOfShader(this, p_fshader); }
}

String ShaderProgram::GetFileExtensionStatic()
{
    return "bshaderprogram";
}

String ShaderProgram::GetFileExtension() const
{
    return ShaderProgram::GetFileExtensionStatic();
}

void ShaderProgram::Read(const XMLNode &xmlInfo)
{
    Asset::Read(xmlInfo);

    Path vShaderFilepath = xmlInfo.GetFilepath("VertexShader");
    if (vShaderFilepath.Exists())
    {
        G_Shader *vShader = ShaderManager::Load(G_Shader::Type::Vertex,
                                              vShaderFilepath);
        SetVertexShader(vShader);
    }

    Path fShaderFilepath = xmlInfo.GetFilepath("FragmentShader");
    if (fShaderFilepath.Exists())
    {
        G_Shader *fShader = ShaderManager::Load(G_Shader::Type::Fragment,
                                              fShaderFilepath);
        SetFragmentShader(fShader);
    }
}

void ShaderProgram::Write(XMLNode *xmlInfo) const
{
    Asset::Write(xmlInfo);

    Path vShaderFilepath = p_vshader ? p_vshader->GetFilepath() : Path();
    xmlInfo->SetFilepath("VertexShader",   vShaderFilepath);

    Path fShaderFilepath = p_fshader ? p_fshader->GetFilepath() : Path();
    xmlInfo->SetFilepath("FragmentShader", fShaderFilepath);
}
