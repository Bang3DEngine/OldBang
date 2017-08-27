#include "Bang/ShaderProgram.h"

#include "Bang/G_Shader.h"
#include "Bang/XMLParser.h"
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
    RetrieveType(vshaderPath, fshaderPath);

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

void ShaderProgram::SetType(ShaderProgram::Type type)
{
    m_type = type;
    Refresh();
}

ShaderProgram::Type ShaderProgram::GetType() const
{
    return m_type;
}

void ShaderProgram::Import(const Path &shaderProgramFilepath)
{
    ReadFromFile(shaderProgramFilepath);
}

void ShaderProgram::OnPreLink()
{
    if (m_type == Type::GBuffer)
    {
        SetVertexInputBinding("B_In_PositionObject", 0);
        SetVertexInputBinding("B_In_NormalObject",   1);
        SetVertexInputBinding("B_In_Uv",             2);
        SetFragmentInputBinding("B_GIn_NormalDepth", 0);
        SetFragmentInputBinding("B_GIn_DiffColor",   1);
        SetFragmentInputBinding("B_GIn_Misc",        2);
        SetFragmentInputBinding("B_GIn_Color",       3);
    }
    else if (m_type == Type::PostProcess)
    {
        SetVertexInputBinding("B_In_PositionObject", 0);
        SetFragmentInputBinding("B_GIn_Color",       0);
    }
}

void ShaderProgram::RetrieveType(const Path &vshaderPath,
                                 const Path &fshaderPath)
{
    String fShaderExt = fshaderPath.GetExtension();
    if      (fShaderExt.EndsWith("_g"))   { m_type = Type::GBuffer; }
    else if (fShaderExt.EndsWith("_pp"))  { m_type = Type::PostProcess; }
    else { m_type = Type::Other; }
}

void ShaderProgram::Read(const XMLNode &xmlInfo)
{
    Asset::Read(xmlInfo);

    if (xmlInfo.Contains("VertexShader"))
    {
        Path vShaderFilepath = xmlInfo.Get<Path>("VertexShader");
        if (vShaderFilepath.Exists())
        {
            G_Shader *vShader = ShaderManager::Load(G_Shader::Type::Vertex,
                                                  vShaderFilepath);
            SetVertexShader(vShader);
        }
    }

    if (xmlInfo.Contains("FragmentShader"))
    {
        Path fShaderFilepath = xmlInfo.Get<Path>("FragmentShader");
        if (fShaderFilepath.Exists())
        {
            G_Shader *fShader = ShaderManager::Load(G_Shader::Type::Fragment,
                                                  fShaderFilepath);
            SetFragmentShader(fShader);
        }
    }
}

void ShaderProgram::Write(XMLNode *xmlInfo) const
{
    Asset::Write(xmlInfo);

    Path vShaderFilepath = p_vshader ? p_vshader->GetResourceFilepath() : Path();
    xmlInfo->Set("VertexShader", vShaderFilepath);

    Path fShaderFilepath = p_fshader ? p_fshader->GetResourceFilepath() : Path();
    xmlInfo->Set("FragmentShader", fShaderFilepath);
}
