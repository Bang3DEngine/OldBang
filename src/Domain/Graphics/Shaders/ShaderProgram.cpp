#include "Bang/ShaderProgram.h"

#include "Bang/glm/glm.hpp"
#include "Bang/glm/gtc/type_ptr.hpp"

#include "Bang/Debug.h"
#include "Bang/Shader.h"
#include "Bang/Texture.h"
#include "Bang/XMLNode.h"
#include "Bang/ShaderManager.h"
#include "Bang/TextureUnitManager.h"

ShaderProgram::ShaderProgram()
{
    m_idGL = glCreateProgram();
}

ShaderProgram::ShaderProgram(const String &vshaderPath,
                             const String &fshaderPath) : ShaderProgram()
{
    Shader *vs = ShaderManager::Load(Shader::Type::Vertex, vshaderPath);
    SetVertexShader(vs);

    Shader *fs = ShaderManager::Load(Shader::Type::Fragment, fshaderPath);
    SetFragmentShader(fs);
}

ShaderProgram::~ShaderProgram()
{
    if (m_vshader) { ShaderManager::UnRegisterUsageOfShader(this, m_vshader); }
    if (m_fshader) { ShaderManager::UnRegisterUsageOfShader(this, m_fshader); }
    glDeleteProgram(m_idGL);
}

String ShaderProgram::GetFileExtensionStatic()
{
    return "bshaderprogram";
}

String ShaderProgram::GetFileExtension() const
{
    return ShaderProgram::GetFileExtensionStatic();
}

bool ShaderProgram::Link()
{
    if (!m_vshader)
    {
        Debug_Error("Vertex shader not set. Can't link shader program.");
        return false;
    }

    if (!m_fshader)
    {
        Debug_Error("Fragment shader not set. Can't link shader program.");
        return false;
    }

    if (m_idGL > 0) { glDeleteProgram(m_idGL); }
    m_idGL = glCreateProgram();

    glAttachShader(m_idGL, m_vshader->GetGLId());
    glAttachShader(m_idGL, m_fshader->GetGLId());
    glLinkProgram(m_idGL);

    GLint linked;
    glGetProgramiv(m_idGL, GL_LINK_STATUS, &linked);
    if (!linked)
    {
       GLint errorLength = 0;
       glGetProgramiv(m_idGL, GL_INFO_LOG_LENGTH, &errorLength);

       if (errorLength > 1)
       {
          char* errorLog = (char*) (malloc(sizeof(char) * errorLength));
          glGetProgramInfoLog(m_idGL, errorLength, NULL, errorLog);

          String errorStr(errorLog);
          Debug_Error("Can't link " << std::endl << this << std::endl <<
                       "   Reason:  " << errorStr);
       }

       glDeleteProgram(m_idGL); m_idGL = 0;
    }

    return linked;
}

GL::BindTarget ShaderProgram::GetGLBindTarget() const
{
    return GL::BindTarget::ShaderProgram;
}

bool ShaderProgram::SetInt(const String &name, int v) const
{
    int location = GetUniformLocation(name);
    if (location >= 0)
    {
        Bind();
        glUniform1i(location, v);
        UnBind();
    }
    return (location >= 0);
}

bool ShaderProgram::SetFloat(const String &name, float v) const
{
    int location = GetUniformLocation(name);
    if (location >= 0)
    {
        Bind();
        glUniform1f(location, v);
        UnBind();
    }
    return (location >= 0);
}

bool ShaderProgram::SetBool(const String &name, bool v) const
{
    return SetInt(name, v ? 1 : 0);
}

bool ShaderProgram::SetVec2 (const String &name, const Vector2& v) const
{
    int location = GetUniformLocation(name);
    if (location >= 0)
    {
        Bind();
        glUniform2fv(location, 1, &v.x);
        UnBind();
    }
    return (location >= 0);
}

bool ShaderProgram::SetVec3 (const String &name, const Vector3& v) const
{
    int location = GetUniformLocation(name);
    if (location >= 0)
    {
        Bind();
        glUniform3fv(location, 1, &v.x);
        UnBind();
    }
    return (location >= 0);
}

bool ShaderProgram::SetVec4 (const String &name, const Vector4& v) const
{
    int location = GetUniformLocation(name);
    if (location >= 0)
    {
        Bind();
        glUniform4fv(location, 1, &v.x);
        UnBind();
    }
    return (location >= 0);
}

bool ShaderProgram::SetColor(const String &name, const Color &c) const
{
    return SetVec4(name, Vector4(c));
}

bool ShaderProgram::SetMat3(const String &name, const glm::mat3 &m) const
{
    int location = GetUniformLocation(name);
    if (location >= 0)
    {
        Bind();
        glUniformMatrix3fv(location, 1, GL_FALSE, &m[0][0]);
        UnBind();
    }
    return (location >= 0);
}

bool ShaderProgram::SetMat4(const String &name, const Matrix4& m) const
{
    int location = GetUniformLocation(name);
    if (location >= 0)
    {
        Bind();
        glUniformMatrix4fv(location, 1, GL_FALSE, m.GetFirstAddress());
        UnBind();
    }
    return (location >= 0);
}

bool ShaderProgram::SetTexture(const String &name, const Texture *texture) const
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
    ENSURE(m_vshader && m_fshader);
    Link();
}

void ShaderProgram::SetVertexShader(Shader *vertexShader)
{
    if (vertexShader->GetType() != Shader::Type::Vertex)
    {
        Debug_Error("You are trying to set as vertex shader a non-vertex shader");
    }

    if (m_vshader) { ShaderManager::UnRegisterUsageOfShader(this, m_vshader); }

    m_vshader = vertexShader;

    if (m_vshader) { ShaderManager::RegisterUsageOfShader(this, m_vshader); }

    Refresh();
}

void ShaderProgram::SetFragmentShader(Shader *fragmentShader)
{
    if (fragmentShader->GetType() != Shader::Type::Fragment)
    {
        Debug_Error("You are trying to set as fragment shader a non-fragment shader");
    }

    if (m_fshader) { ShaderManager::UnRegisterUsageOfShader(this, m_fshader); }

    m_fshader = fragmentShader;

    if (m_fshader) { ShaderManager::RegisterUsageOfShader(this, m_fshader); }

    Refresh();
}

Shader *ShaderProgram::GetVertexShader() const
{
    return m_vshader;
}

Shader *ShaderProgram::GetFragmentShader() const
{
    return m_fshader;
}

GLint ShaderProgram::GetUniformLocation(const String &name) const
{
    auto it = m_nameToLocationCache.Find(name);
    if (it != m_nameToLocationCache.End()) { return it->second; }

    const GLuint location = glGetUniformLocation(m_idGL, name.ToCString());
    m_nameToLocationCache[name] = location;
    return location;
}

GLint ShaderProgram::GetAttribLocation(const String &name) const
{
    return glGetAttribLocation(m_idGL, name.ToCString());
}

String ShaderProgram::ToString() const
{
    std::ostringstream oss;
    oss << "Shader program: " << std::endl <<
           "   " << m_vshader << std::endl <<
           "   " << m_fshader << std::endl;
    return oss.str();
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
            GLuint texUnit = TextureUnitManager::BindTexture(texture);
            glUniform1i(location, texUnit);
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

void ShaderProgram::Read(const XMLNode &xmlInfo)
{
    Asset::Read(xmlInfo);

    String vShaderFilepath = xmlInfo.GetFilepath("VertexShader");
    if (IO::ExistsFile(vShaderFilepath))
    {
        Shader *vShader = new Shader(Shader::Type::Vertex, vShaderFilepath);
        SetVertexShader(vShader);
    }

    String fShaderFilepath = xmlInfo.GetFilepath("FragmentShader");
    if (IO::ExistsFile(fShaderFilepath))
    {
        Shader *fShader = new Shader(Shader::Type::Fragment, fShaderFilepath);
        SetFragmentShader(fShader);
    }
}

void ShaderProgram::Write(XMLNode *xmlInfo) const
{
    Asset::Write(xmlInfo);

    String vShaderFilepath = m_vshader ? m_vshader->GetFilepath() : "";
    xmlInfo->SetFilepath("VertexShader",   vShaderFilepath, "vert glsl");

    String fShaderFilepath = m_fshader ? m_fshader->GetFilepath() : "";
    xmlInfo->SetFilepath("FragmentShader", fShaderFilepath, "frag glsl");
}
