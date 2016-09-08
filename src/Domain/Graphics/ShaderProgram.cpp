#include "ShaderProgram.h"

ShaderProgram::ShaderProgram() : m_vshader(nullptr), m_fshader(nullptr)
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
    this->m_vshader = vshader;
}

void ShaderProgram::BindFragmentShader(Shader *fshader)
{
    this->m_fshader = fshader;
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

          free(errorLog);
        }

        glDeleteProgram(m_idGL);
    }

    return linked;
}

ShaderProgram::Type ShaderProgram::GetType() const
{
    if (m_fshader)
    {
        String name = Persistence::GetFileName(m_fshader->GetFilepath());
        if (name.BeginsWith("FWD_"))
        {
            return Type::Forward;
        }
        else if (name.BeginsWith("D2G_"))
        {
            return Type::DrawToGBuffer;
        }
        else if (name.BeginsWith("PR_"))
        {
            return Type::PostRender;
        }
    }

    return Type::Other;
}

bool ShaderProgram::SetUniformFloat(const String &name, float v, bool warn) const
{
    int location = GetUniformLocation(name);
    if (location >= 0)
    {
        Bind();
        glUniform1fv(location, 1, &v);
        UnBind();
    }
    else
    {
        if (warn) Debug_Warn("Couldn't find uniform '" + name + "' in " <<
                                std::endl << this << std::endl << " . Not setting it.");
    }

    return (location >= 0);
}

bool ShaderProgram::SetUniformVec2 (const String &name, const Vector2& v, bool warn) const
{
    int location = GetUniformLocation(name);
    if (location >= 0)
    {
        Bind();
        glUniform2fv(location, 1, &v.x);
        UnBind();
    }
    else
    {
        if (warn) Debug_Warn("Couldn't find uniform '" + name + "' in " <<
                                std::endl << this << std::endl << " . Not setting it.");
    }

    return (location >= 0);
}

bool ShaderProgram::SetUniformVec3 (const String &name, const Vector3& v, bool warn) const
{
    int location = GetUniformLocation(name);
    if (location >= 0)
    {
        Bind();
        glUniform3fv(location, 1, &v.x);
        UnBind();
    }
    else
    {
        if (warn) Debug_Warn("Couldn't find uniform '" + name + "' in " <<
                                std::endl << this << std::endl << " . Not setting it.");
    }

    return (location >= 0);
}

bool ShaderProgram::SetUniformVec4 (const String &name, const Vector4& v, bool warn) const
{
    int location = GetUniformLocation(name);
    if (location >= 0)
    {
        Bind();
        glUniform4fv(location, 1, &v.x);
        UnBind();
    }
    else
    {
        if (warn) Debug_Warn("Couldn't find uniform '" + name + "' in " <<
                                std::endl << this << std::endl << " . Not setting it.");
    }

    return (location >= 0);
}

bool ShaderProgram::SetUniformColor(const String &name, const Color &c, bool warn) const
{
    return SetUniformVec4(name, Vector4(c), warn);
}

bool ShaderProgram::SetUniformMat4 (const String &name, const Matrix4& m, bool warn) const
{
    int location = GetUniformLocation(name);
    if (location >= 0)
    {
        Bind();
        glUniformMatrix4fv(location, 1, GL_FALSE, m.GetFirstAddress());
        UnBind();
    }
    else
    {
        if (warn) Debug_Warn("Couldn't find uniform '" + name + "' in " <<
                                std::endl << this << std::endl << " . Not setting it.");
    }

    return (location >= 0);
}

bool ShaderProgram::SetUniformTexture(const String &name, const Texture *texture, bool warn) const
{
    int location = GetUniformLocation(name);
    if (location >= 0)
    {
        m_names_To_Textures[name] = texture;
    }
    else
    {
        if (warn) Debug_Warn("Couldn't find uniform '" + name + "' in " <<
                                std::endl << this << std::endl << " . Not setting it.");
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

const String ShaderProgram::ToString() const
{
    std::ostringstream oss;
    oss << "Shader program: " << std::endl <<
           "   " << m_vshader << std::endl <<
           "   " << m_fshader << std::endl;
    return oss.str();
}

void ShaderProgram::Bind() const
{
    PreBind(GL_CURRENT_PROGRAM);
    glUseProgram(m_idGL);

    // Bind textures at the moment we bind the shader program
    int textureUnit = 0;
    for (auto it = m_names_To_Textures.Begin(); it != m_names_To_Textures.End(); ++it)
    {
        String texName = it->first;
        const Texture *tex = it->second;
        if (tex)
        {
            int location = GetUniformLocation(texName);

            glUniform1i(location, textureUnit);

            //Set the uniform with the texture slot
            tex->BindToTextureUnit(textureUnit); //Leave it bound
            textureUnit++;
        }
    }
}

void ShaderProgram::UnBind() const
{
    glUseProgram(PreUnBind(GL_CURRENT_PROGRAM));
}

