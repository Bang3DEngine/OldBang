#include "ShaderProgram.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Debug.h"
#include "Shader.h"
#include "Texture.h"

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

void ShaderProgram::OnRenderingStarts(GameObject *go, ShaderProgram *sp)
{
    GL::Bind(this);
}

void ShaderProgram::OnRenderingEnds(GameObject *go, ShaderProgram *sp)
{
    GL::UnBind(this);
}

GL::BindTarget ShaderProgram::GetGLBindTarget() const
{
    return GL::BindTarget::ShaderProgram;
}

bool ShaderProgram::SetFloat(const String &name, float v) const
{
    int location = GetUniformLocation(name);
    if (location >= 0)
    {
        Bind();
        glUniform1fv(location, 1, &v);
        UnBind();
    }
    return (location >= 0);
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
    int location = GetUniformLocation(name);
    if (location >= 0)
    {
        m_names_To_Texture[name] = texture;
        UpdateTextureBindings();
    }
    return (location >= 0);
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
    return glGetUniformLocation(m_idGL, name.ToCString());
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

void ShaderProgram::Bind() const
{
    GL::Bind(this);
    UpdateTextureBindings();
}

void ShaderProgram::UnBind() const
{
    for (auto it = m_names_To_Texture.Begin();
         it != m_names_To_Texture.End(); ++it)
    {
        const Texture *tex = it->second;
        if (tex)
        {
            tex->UnBind();
        }
    }
    GL::UnBind(this);
}

void ShaderProgram::UpdateTextureBindings() const
{
    int textureUnit = 0;
    for (auto it = m_names_To_Texture.Begin();
         it != m_names_To_Texture.End(); ++it)
    {
        const String &texName = it->first;
        const Texture *tex = it->second;

        if (tex)
        {
            int location = GetUniformLocation(texName);
            // Set the uniform with the corresponding texture unit
            glUniform1i(location, textureUnit);

            // Bind To texture unit
            tex->BindToTextureUnit(textureUnit); //Leave it bound
            textureUnit++;
        }
    }
}

