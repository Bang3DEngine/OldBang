#include "ShaderProgram.h"

ShaderProgram::ShaderProgram() : vshader(nullptr), fshader(nullptr)
{
    idgl = glCreateProgram();
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(idgl);
}

void ShaderProgram::BindVertexShader(Shader *vshader)
{
    this->vshader = vshader;
}

void ShaderProgram::BindFragmentShader(Shader *fshader)
{
    this->fshader = fshader;
}

bool ShaderProgram::Link()
{
    if(vshader == nullptr)
    {
        Logger_Error("Vertex shader not set. Can't link shader program.");
        return false;
    }

    if(fshader == nullptr)
    {
        Logger_Error("Fragment shader not set. Can't link shader program.");
        return false;
    }

    glAttachShader(idgl, vshader->GetGLId());
    glAttachShader(idgl, fshader->GetGLId());

    glLinkProgram(idgl);

    GLint linked;
    glGetProgramiv(idgl, GL_LINK_STATUS, &linked);
    if(not linked)
    {
       GLint errorLength = 0;
       glGetProgramiv(idgl, GL_INFO_LOG_LENGTH, &errorLength);

       if(errorLength > 1)
        {
          char* errorLog = (char*) (malloc(sizeof(char) * errorLength));
          glGetProgramInfoLog(idgl, errorLength, NULL, errorLog);

          std::string errorStr(errorLog);
          Logger_Error("Can't link shader program: " + errorStr);

          free(errorLog);
        }

        glDeleteProgram(idgl);
        return false;
    }

    return true;
}

bool ShaderProgram::SetUniformFloat(const std::string &name, float v, bool warn) const
{
    int location = glGetUniformLocation(idgl, name.c_str());
    if(location >= 0)
    {
        this->Bind();
        glUniform1fv(location, 1, &v);
        this->UnBind();
        return true;
    }
    else
    {
        if(warn) Logger_Warning("Couldn't find uniform '" + name + "' in shader program. Not setting it.");
        return false;
    }
}

bool ShaderProgram::SetUniformVec2 (const std::string &name, const glm::vec2& v, bool warn) const
{
    int location = glGetUniformLocation(idgl, name.c_str());
    if(location >= 0)
    {
        this->Bind();
        glUniform2fv(location, 1, &v[0]);
        this->UnBind();
        return true;
    }
    else
    {
        if(warn) Logger_Warning("Couldn't find uniform '" + name + "' in shader program. Not setting it.");
        return false;
    }
}

bool ShaderProgram::SetUniformVec3 (const std::string &name, const glm::vec3& v, bool warn) const
{
    int location = glGetUniformLocation(idgl, name.c_str());
    if(location >= 0)
    {
        this->Bind();
        glUniform3fv(location, 1, &v[0]);
        this->UnBind();
        return true;
    }
    else
    {
        if(warn) Logger_Warning("Couldn't find uniform '" + name + "' in shader program. Not setting it.");
        return false;
    }
}

bool ShaderProgram::SetUniformVec4 (const std::string &name, const glm::vec4& v, bool warn) const
{
    int location = glGetUniformLocation(idgl, name.c_str());
    if(location >= 0)
    {
        this->Bind();
        glUniform4fv(location, 1, &v[0]);
        this->UnBind();
        return true;
    }
    else
    {
        if(warn) Logger_Warning("Couldn't find uniform '" + name + "' in shader program. Not setting it.");
        return false;
    }
}

bool ShaderProgram::SetUniformMat4 (const std::string &name, const glm::mat4& m, bool warn) const
{
    int location = glGetUniformLocation(idgl, name.c_str());
    if(location >= 0)
    {
        this->Bind();
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(m));
        this->UnBind();
        return true;
    }
    else
    {
        if(warn) Logger_Warning("Couldn't find uniform '" + name + "' in shader program. Not setting it.");
        return false;
    }
}

bool ShaderProgram::SetUniformTexture2D(const std::string &name, const Texture *texture, bool warn) const
{
    int location = glGetUniformLocation(idgl, name.c_str());
    if(location >= 0)
    {
        this->Bind();
        glUniform1i(location, texture->GetTextureSlot());
        this->UnBind();
        return true;
    }
    else
    {
        if(warn) Logger_Warning("Couldn't find uniform '" + name + "' in shader program. Not setting it.");
        return false;
    }
}

void ShaderProgram::Bind() const
{
    PreBind(GL_CURRENT_PROGRAM);
    glUseProgram(idgl);
}

void ShaderProgram::UnBind() const
{
    glUseProgram(PreUnBind());
}


