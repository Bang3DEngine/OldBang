#include "Bang/G_ShaderProgram.h"

#include "Bang/Debug.h"
#include "Bang/Color.h"
#include "Bang/Vector2.h"
#include "Bang/Vector3.h"
#include "Bang/Vector4.h"
#include "Bang/Matrix3.h"
#include "Bang/Matrix4.h"
#include "Bang/G_Shader.h"
#include "Bang/G_Texture.h"
#include "Bang/G_TextureUnitManager.h"

G_ShaderProgram::G_ShaderProgram()
{
    m_idGL = glCreateProgram();
}

void G_ShaderProgram::Load(const Path &vshaderPath, const Path &fshaderPath)
{
    G_Shader *vs = new G_Shader(G_Shader::Type::Vertex);
    vs->LoadFromFile(vshaderPath);
    SetVertexShader(vs);

    G_Shader *fs = new G_Shader(G_Shader::Type::Fragment);
    vs->LoadFromFile(fshaderPath);
    SetFragmentShader(fs);
}


G_ShaderProgram::~G_ShaderProgram()
{
    glDeleteProgram(m_idGL);
}

bool G_ShaderProgram::Link()
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

    if (m_idGL > 0) { glDeleteProgram(m_idGL); }
    m_idGL = glCreateProgram();

    glAttachShader(m_idGL, p_vshader->GetGLId());
    glAttachShader(m_idGL, p_fshader->GetGLId());

    OnPreLink();
    glLinkProgram(m_idGL);

    GLint linked;
    glGetProgramiv(m_idGL, GL_LINK_STATUS, &linked);
    if (!linked)
    {
       Debug_Error("The shader program " << this << " did not link");

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
       return false;
    }

    m_nameToLocationCache.Clear(); // Invalidate cache
    return true;
}

GL::BindTarget G_ShaderProgram::GetGLBindTarget() const
{
    return GL::BindTarget::ShaderProgram;
}

bool G_ShaderProgram::Set(const String &name, int v) const
{
    ASSERT(GL::IsBound(this));
    int location = GetUniformLocation(name);
    if (location >= 0)
    {
        glUniform1i(location, v);
    }
    return (location >= 0);
}

bool G_ShaderProgram::Set(const String &name, float v) const
{
    ASSERT(GL::IsBound(this));
    int location = GetUniformLocation(name);
    if (location >= 0)
    {
        glUniform1f(location, v);
    }
    return (location >= 0);
}

bool G_ShaderProgram::Set(const String &name, bool v) const
{
    return Set(name, v ? 1 : 0);
}

bool G_ShaderProgram::Set(const String &name, const Matrix3 &m) const
{
    ASSERT(GL::IsBound(this));
    int location = GetUniformLocation(name);
    if (location >= 0)
    {
        glUniformMatrix3fv(location, 1, GL_FALSE, m.Data());
    }
    return (location >= 0);
}

bool G_ShaderProgram::Set(const String &name, const Matrix4 &m) const
{
    ASSERT(GL::IsBound(this));
    int location = GetUniformLocation(name);
    if (location >= 0)
    {
        glUniformMatrix4fv(location, 1, GL_FALSE, m.Data());
    }
    return (location >= 0);
}

bool G_ShaderProgram::Set(const String &name, const Vector2 &v) const
{
    ASSERT(GL::IsBound(this));
    int location = GetUniformLocation(name);
    if (location >= 0)
    {
        glUniform2fv(location, 1, v.Data());
    }
    return (location >= 0);
}

bool G_ShaderProgram::Set(const String &name, const Vector3 &v) const
{
    ASSERT(GL::IsBound(this));
    int location = GetUniformLocation(name);
    if (location >= 0)
    {
        glUniform3fv(location, 1, v.Data());
    }
    return (location >= 0);
}

bool G_ShaderProgram::Set(const String &name, const Vector4 &v) const
{
    ASSERT(GL::IsBound(this));
    int location = GetUniformLocation(name);
    if (location >= 0)
    {
        glUniform4fv(location, 1, v.Data());
    }
    return (location >= 0);
}

bool G_ShaderProgram::Set(const String &name, const Color &c) const
{
    return Set(name, Vector4(c.r, c.g, c.b, c.a));
}

bool G_ShaderProgram::Set(const String &name,  const G_Texture *texture) const
{
    bool uniformIsUsed = BindTextureToAvailableUnit(name, texture);
    if (uniformIsUsed)
    {
        m_namesToTexture[name] = texture;
    }
    return uniformIsUsed;
}

void G_ShaderProgram::Refresh()
{
    ENSURE(p_vshader && p_fshader);
    Link();
}

void G_ShaderProgram::SetVertexShader(G_Shader *vertexShader)
{
    if (vertexShader->GetType() != G_Shader::Type::Vertex)
    {
        Debug_Error("You are trying to set as vertex shader a non-vertex shader");
    }
    p_vshader = vertexShader;
    Refresh();
}

void G_ShaderProgram::SetFragmentShader(G_Shader *fragmentShader)
{
    if (fragmentShader->GetType() != G_Shader::Type::Fragment)
    {
        Debug_Error("You are trying to set as fragment shader a non-fragment shader");
    }
    p_fshader = fragmentShader;
    Refresh();
}

void G_ShaderProgram::SetVertexInputBinding(const String &inputName, uint location)
{
    glBindAttribLocation(m_idGL, location, inputName.ToCString());
}

void G_ShaderProgram::SetFragmentInputBinding(const String &inputName, uint location)
{
    glBindFragDataLocation(m_idGL, location, inputName.ToCString());
}

G_Shader *G_ShaderProgram::GetVertexShader() const
{
    return p_vshader;
}

G_Shader *G_ShaderProgram::GetFragmentShader() const
{
    return p_fshader;
}

GLint G_ShaderProgram::GetUniformLocation(const String &name) const
{
    auto it = m_nameToLocationCache.Find(name);
    if (it != m_nameToLocationCache.End()) { return it->second; }

    const GLuint location = glGetUniformLocation(m_idGL, name.ToCString());
    m_nameToLocationCache[name] = location;
    return location;
}

GLint G_ShaderProgram::GetAttribLocation(const String &name) const
{
    return glGetAttribLocation(m_idGL, name.ToCString());
}

bool G_ShaderProgram::BindTextureToAvailableUnit(const String &texName,
                                                 const G_Texture *texture) const
{
    int location = -1;
    if (texture)
    {
        location = GetUniformLocation(texName);
        if (location >= 0)
        {
            GLuint texUnit = G_TextureUnitManager::BindTexture(texture);
            glUniform1i(location, texUnit);
        }
    }
    return (location >= 0);
}

void G_ShaderProgram::Bind() const
{
    GL::Bind(this);
    UpdateTextureBindings();
}

void G_ShaderProgram::UnBind() const
{
    GL::UnBind(this);
}

void G_ShaderProgram::UpdateTextureBindings() const
{
    for (auto it = m_namesToTexture.Begin(); it != m_namesToTexture.End(); ++it)
    {
        BindTextureToAvailableUnit(it->first, it->second);
    }
}

void G_ShaderProgram::OnPreLink()
{
}
