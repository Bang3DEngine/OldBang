#include "ShaderProgram.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Debug.h"
#include "Shader.h"
#include "Texture.h"
#include "TextureUnitManager.h"

ShaderProgram::ShaderProgram()
{
    m_idGL = glCreateProgram();
}

ShaderProgram::ShaderProgram(const String &vshaderPath,
                             const String &fshaderPath) : ShaderProgram()
{
    Shader *vs = new Shader(Shader::Type::Vertex, vshaderPath);
    BindVertexShader(vs);
    Shader *fs = new Shader(Shader::Type::Fragment, fshaderPath);
    BindFragmentShader(fs);
    Link();
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(m_idGL);
}

void ShaderProgram::BindVertexShader(Shader *vshader)
{
    m_vshader = vshader;
}

void ShaderProgram::BindFragmentShader(Shader *fshader)
{
    m_fshader = fshader;
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

        glDeleteProgram(m_idGL);
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
    if (location >= 0) { m_nameToLocationCache[name] = location; }
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

