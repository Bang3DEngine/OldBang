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
        Logger::Error("Vertex shader not set. Can't link shader program.");
        return false;
    }

    if(fshader == nullptr)
    {
        Logger::Error("Fragment shader not set. Can't link shader program.");
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
          Logger::Error("Can't link shader program: " + errorStr);

          free(errorLog);
        }

        glDeleteProgram(idgl);
        return false;
    }

    return true;
}

void ShaderProgram::SetUniformFloat(const std::string &name, float v)
{
    int location = glGetUniformLocation(idgl, name.c_str());
    this->Bind();
    glUniform1fv(location, 1, &v);
    this->UnBind();
}

void ShaderProgram::SetUniformVec2 (const std::string &name, const glm::vec2& v)
{
    int location = glGetUniformLocation(idgl, name.c_str());
    this->Bind();
    glUniform2fv(location, 1, &v[0]);
    this->UnBind();
}

void ShaderProgram::SetUniformVec3 (const std::string &name, const glm::vec3& v)
{
    int location = glGetUniformLocation(idgl, name.c_str());
    this->Bind();
    glUniform3fv(location, 1, &v[0]);
    this->UnBind();
}

void ShaderProgram::SetUniformVec4 (const std::string &name, const glm::vec4& v)
{
    int location = glGetUniformLocation(idgl, name.c_str());
    this->Bind();
    glUniform4fv(location, 1, &v[0]);
    this->UnBind();
}

void ShaderProgram::SetUniformMat4 (const std::string &name, const glm::mat4& m)
{
    int location = glGetUniformLocation(idgl, name.c_str());
    this->Bind();
    glUniformMatrix4fv(location, 1, GL_FALSE, &m[0][0]);
    this->UnBind();
}


void ShaderProgram::Bind() const
{
    if(lastBoundId == -1) glGetIntegerv(GL_CURRENT_PROGRAM, &lastBoundId);
    glUseProgram(idgl);
}

void ShaderProgram::UnBind() const
{
    //needed if, if UnBind called multiple times
    if(lastBoundId != -1) glUseProgram(lastBoundId);
    lastBoundId = -1;
}


