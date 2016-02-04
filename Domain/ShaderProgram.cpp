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


