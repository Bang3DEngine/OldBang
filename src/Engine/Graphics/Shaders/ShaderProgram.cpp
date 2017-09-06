#include "Bang/ShaderProgram.h"

#include "Bang/Debug.h"
#include "Bang/Color.h"
#include "Bang/Shader.h"
#include "Bang/Vector2.h"
#include "Bang/Vector3.h"
#include "Bang/Vector4.h"
#include "Bang/Matrix3.h"
#include "Bang/Matrix4.h"
#include "Bang/XMLNode.h"
#include "Bang/Texture.h"
#include "Bang/ShaderManager.h"
#include "Bang/TextureUnitManager.h"

ShaderProgram::ShaderProgram()
{
    m_idGL = GL::CreateProgram();
}

void ShaderProgram::Load(const Path &vshaderPath, const Path &fshaderPath)
{
    RetrieveType(vshaderPath, fshaderPath);

    Shader *vs = ShaderManager::Load(GL::ShaderType::Vertex, vshaderPath);
    SetVertexShader(vs);

    Shader *fs = ShaderManager::Load(GL::ShaderType::Fragment, fshaderPath);
    SetFragmentShader(fs);
}


ShaderProgram::~ShaderProgram()
{
    if (p_vshader) { ShaderManager::UnRegisterUsageOfShader(this, p_vshader); }
    if (p_fshader) { ShaderManager::UnRegisterUsageOfShader(this, p_fshader); }
    GL::DeleteProgram(m_idGL);
}

bool ShaderProgram::Link()
{
    if (!p_vshader)
    {
        Debug_Error("Vertex shader not set. Can't link shader program.");
        return false;
    }

    if (!p_fshader)
    {
        Debug_Error("Fragment shader not set. Can't link shader program.");
        return false;
    }

    if (m_idGL > 0) { GL::DeleteProgram(m_idGL); }
    m_idGL = GL::CreateProgram();

    GL::AttachShader(m_idGL, p_vshader->GetGLId());
    GL::AttachShader(m_idGL, p_fshader->GetGLId());

    if (GetInputType() == InputType::GBuffer)
    {
        SetVertexInputBinding("B_In_PositionObject", 0);
        SetVertexInputBinding("B_In_NormalObject",   1);
        SetVertexInputBinding("B_In_Uv",             2);
        SetFragmentInputBinding("B_GIn_NormalDepth", 0);
        SetFragmentInputBinding("B_GIn_DiffColor",   1);
        SetFragmentInputBinding("B_GIn_Misc",        2);
        SetFragmentInputBinding("B_GIn_Color",       3);
    }
    else if (GetInputType() == InputType::PostProcess)
    {
        SetVertexInputBinding("B_In_PositionObject", 0);
        SetFragmentInputBinding("B_GIn_Color",       0);
    }

    if (!GL::LinkProgram(m_idGL))
    {
       Debug_Error("The shader program " << this << " did not link: " <<
                   GL::GetProgramLinkErrorMsg(m_idGL));
       GL::DeleteProgram(m_idGL); m_idGL = 0;
       return false;
    }

    m_nameToLocationCache.Clear(); // Invalidate cache
    return true;
}

GL::BindTarget ShaderProgram::GetGLBindTarget() const
{
    return GL::BindTarget::ShaderProgram;
}

bool ShaderProgram::Set(const String &name, const Texture *texture) const
{
    bool uniformIsUsed = BindTextureToAvailableUnit(name, texture);
    if (uniformIsUsed)
    {
        m_namesToTexture[name] = texture;
    }
    return uniformIsUsed;
}

void ShaderProgram::Refresh()
{
    ENSURE(p_vshader && p_fshader);
    Link();
}

void ShaderProgram::SetVertexShader(Shader *vertexShader)
{
    if (p_vshader) { ShaderManager::UnRegisterUsageOfShader(this, p_vshader); }
    if (vertexShader->GetType() != GL::ShaderType::Vertex)
    {
        Debug_Error("You are trying to set as vertex shader a non-vertex shader");
    }
    p_vshader = vertexShader;
    if (p_vshader) { ShaderManager::RegisterUsageOfShader(this, p_vshader); }
    Refresh();
}

void ShaderProgram::SetFragmentShader(Shader *fragmentShader)
{
    if (p_fshader) { ShaderManager::UnRegisterUsageOfShader(this, p_fshader); }
    if (fragmentShader->GetType() != GL::ShaderType::Fragment)
    {
        Debug_Error("You are trying to set as fragment shader a non-fragment shader");
    }
    p_fshader = fragmentShader;
    if (p_fshader) { ShaderManager::RegisterUsageOfShader(this, p_fshader); }
    Refresh();
}

void ShaderProgram::SetInputType(ShaderProgram::InputType inputType)
{
    m_inputType = inputType;
    Refresh();
}

void ShaderProgram::SetVertexInputBinding(const String &attribName,
                                            uint location)
{
    GL::BindAttribLocation(m_idGL, location, attribName);
}

void ShaderProgram::SetFragmentInputBinding(const String &fragDataName,
                                              uint location)
{
    GL::BindFragDataLocation(m_idGL, location, fragDataName);
}

ShaderProgram::InputType ShaderProgram::GetInputType() const
{
    return m_inputType;
}

Shader *ShaderProgram::GetVertexShader() const
{
    return p_vshader;
}

Shader *ShaderProgram::GetFragmentShader() const
{
    return p_fshader;
}

GLint ShaderProgram::GetUniformLocation(const String &name) const
{
    auto it = m_nameToLocationCache.Find(name);
    if (it != m_nameToLocationCache.End()) { return it->second; }

    const GLuint location = GL::GetUniformLocation(m_idGL, name);
    m_nameToLocationCache[name] = location;
    return location;
}

void ShaderProgram::Import(const Path &shaderProgramFilepath)
{
    ImportXMLFromFile(shaderProgramFilepath);
}

void ShaderProgram::RetrieveType(const Path &vshaderPath,
                                 const Path &fshaderPath)
{
    String fShaderExt = fshaderPath.GetExtension();
    if      (fShaderExt.EndsWith("_g"))   { m_inputType = InputType::GBuffer; }
    else if (fShaderExt.EndsWith("_pp"))  { m_inputType = InputType::PostProcess; }
    else { m_inputType = InputType::Other; }
}

void ShaderProgram::ImportXML(const XMLNode &xmlInfo)
{
    Asset::ImportXML(xmlInfo);

    if (xmlInfo.Contains("VertexShader"))
    {
        Path vShaderFilepath = xmlInfo.Get<Path>("VertexShader");
        if (vShaderFilepath.Exists())
        {
            Shader *vShader = ShaderManager::Load(GL::ShaderType::Vertex,
                                                  vShaderFilepath);
            SetVertexShader(vShader);
        }
    }

    if (xmlInfo.Contains("FragmentShader"))
    {
        Path fShaderFilepath = xmlInfo.Get<Path>("FragmentShader");
        if (fShaderFilepath.Exists())
        {
            Shader *fShader = ShaderManager::Load(GL::ShaderType::Fragment,
                                                  fShaderFilepath);
            SetFragmentShader(fShader);
        }
    }
}

void ShaderProgram::ExportXML(XMLNode *xmlInfo) const
{
    Asset::ExportXML(xmlInfo);

    Path vShaderFilepath = p_vshader ? p_vshader->GetResourceFilepath() : Path();
    xmlInfo->Set("VertexShader", vShaderFilepath);

    Path fShaderFilepath = p_fshader ? p_fshader->GetResourceFilepath() : Path();
    xmlInfo->Set("FragmentShader", fShaderFilepath);
}

bool ShaderProgram::BindTextureToAvailableUnit(const String &texName,
                                                 const Texture *texture) const
{
    int location = -1;
    if (texture)
    {
        location = GetUniformLocation(texName);
        if (location >= 0)
        {
            int texUnit = TextureUnitManager::BindTexture(texture);
            GL::Uniform(location, texUnit);
        }
    }
    return (location >= 0);
}

void ShaderProgram::Bind() const
{
    GL::Bind(this);
    UpdateTextureBindings();
}

void ShaderProgram::UnBind() const
{
    GL::UnBind(this);
}

void ShaderProgram::UpdateTextureBindings() const
{
    for (auto it = m_namesToTexture.Begin(); it != m_namesToTexture.End(); ++it)
    {
        BindTextureToAvailableUnit(it->first, it->second);
    }
}
