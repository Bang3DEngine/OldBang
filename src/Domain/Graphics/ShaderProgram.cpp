#include "ShaderProgram.h"

ShaderProgram::ShaderProgram() : p_vshader(nullptr), p_fshader(nullptr)
{
    m_idGL = glCreateProgram();
}

ShaderProgram::ShaderProgram(const std::string &vshaderPath,
                             const std::string &fshaderPath) : ShaderProgram()
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
    this->p_vshader = vshader;
}

void ShaderProgram::BindFragmentShader(Shader *fshader)
{
    this->p_fshader = fshader;
}

bool ShaderProgram::Link()
{
    if (!p_vshader)
    {
        Logger_Error("Vertex shader not set. Can't link shader program.");
        return false;
    }

    if (!p_fshader)
    {
        Logger_Error("Fragment shader not set. Can't link shader program.");
        return false;
    }

    glAttachShader(m_idGL, p_vshader->GetGLId());
    glAttachShader(m_idGL, p_fshader->GetGLId());

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

          std::string errorStr(errorLog);
          Logger_Error("Can't link " << std::endl << this << std::endl <<
                       "   Reason:  " << errorStr);

          free(errorLog);
        }

        glDeleteProgram(m_idGL);
    }

    return linked;
}

bool ShaderProgram::SetUniformFloat(const std::string &name, float v, bool warn) const
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
        if (warn) Logger_Warn("Couldn't find uniform '" + name + "' in " <<
                                std::endl << this << std::endl << " . Not setting it.");
    }

    return (location >= 0);
}

bool ShaderProgram::SetUniformVec2 (const std::string &name, const glm::vec2& v, bool warn) const
{
    int location = GetUniformLocation(name);
    if (location >= 0)
    {
        Bind();
        glUniform2fv(location, 1, &v[0]);
        UnBind();
    }
    else
    {
        if (warn) Logger_Warn("Couldn't find uniform '" + name + "' in " <<
                                std::endl << this << std::endl << " . Not setting it.");
    }

    return (location >= 0);
}

bool ShaderProgram::SetUniformVec3 (const std::string &name, const Vector3& v, bool warn) const
{
    int location = GetUniformLocation(name);
    if (location >= 0)
    {
        Bind();
        glUniform3fv(location, 1, &v[0]);
        UnBind();
    }
    else
    {
        if (warn) Logger_Warn("Couldn't find uniform '" + name + "' in " <<
                                std::endl << this << std::endl << " . Not setting it.");
    }

    return (location >= 0);
}

bool ShaderProgram::SetUniformVec4 (const std::string &name, const glm::vec4& v, bool warn) const
{
    int location = GetUniformLocation(name);
    if (location >= 0)
    {
        Bind();
        glUniform4fv(location, 1, &v[0]);
        UnBind();
    }
    else
    {
        if (warn) Logger_Warn("Couldn't find uniform '" + name + "' in " <<
                                std::endl << this << std::endl << " . Not setting it.");
    }

    return (location >= 0);
}

bool ShaderProgram::SetUniformMat4 (const std::string &name, const Matrix4& m, bool warn) const
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
        if (warn) Logger_Warn("Couldn't find uniform '" + name + "' in " <<
                                std::endl << this << std::endl << " . Not setting it.");
    }

    return (location >= 0);
}

bool ShaderProgram::SetUniformTexture(const std::string &name, Texture *texture, bool warn) const
{
    int location = GetUniformLocation(name);
    if (location >= 0)
    {
        m_namesToTextures[name] = texture;
    }
    else
    {
        if (warn) Logger_Warn("Couldn't find uniform '" + name + "' in " <<
                                std::endl << this << std::endl << " . Not setting it.");
    }

    return (location >= 0);
}

Shader *ShaderProgram::GetVertexShader() const
{
    return p_vshader;
}

Shader *ShaderProgram::GetFragmentShader() const
{
    return p_fshader;
}

GLint ShaderProgram::GetUniformLocation(const std::string &name) const
{
    glGetUniformLocation(m_idGL, name.c_str());
}

GLint ShaderProgram::GetAttribLocation(const std::string &name) const
{
    return glGetAttribLocation(m_idGL, name.c_str());
}

const std::string ShaderProgram::ToString() const
{
    std::ostringstream oss;
    oss << "Shader program: " << std::endl <<
           "   " << p_vshader << std::endl <<
           "   " << p_fshader << std::endl;
    return oss.str();
}

void ShaderProgram::Bind() const
{
    PreBind(GL_CURRENT_PROGRAM);
    glUseProgram(m_idGL);

    // Bind textures at the moment we bind the shader program
    int textureUnit = 0;
    for (auto it = m_namesToTextures.begin(); it != m_namesToTextures.end(); ++it)
    {
        std::string texName = it->first;
        Texture *tex = it->second;
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

void ShaderProgram::Write(std::ostream &f) const
{
}

void ShaderProgram::Read(std::istream &f)
{
}
