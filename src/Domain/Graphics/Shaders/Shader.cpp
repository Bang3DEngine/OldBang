#include "Bang/Shader.h"

#include <sstream>
#include <fstream>
#include <iostream>

#include "Bang/Debug.h"
#include "Bang/IO.h"
#include "Bang/ShaderPreprocessor.h"

Shader::Shader(Shader::Type t)
    : m_sourceCode(""), m_type(t)
{
}

Shader::Shader(Shader::Type t, const Path &shaderPath) : Shader(t)
{
    LoadFromFile(shaderPath);
}

bool Shader::LoadFromFile(const Path& filepath)
{
    if (!filepath.Exists())
    {
        Debug_Error("Shader '" << filepath << "' does not exist.");
        return false;
    }

    m_filepath = filepath;
    m_sourceCode = IO::GetFileContents(m_filepath.GetAbsolute());
    ShaderPreprocessor::PreprocessCode(&m_sourceCode);

    m_idGL = glCreateShader(GLint(m_type));

    const GLchar *source = (const GLchar*)(m_sourceCode.ToCString());
    GLint size = m_sourceCode.Length();
    glShaderSource(m_idGL, 1, &source, &size);
    glCompileShader(m_idGL);

    GLint ok;
    glGetShaderiv(m_idGL, GL_COMPILE_STATUS, &ok);
    if (!ok)
    {
        GLint maxLength = 0;
        glGetShaderiv(m_idGL, GL_INFO_LOG_LENGTH, &maxLength);

        Array<GLchar> v(maxLength);
        glGetShaderInfoLog(m_idGL, maxLength, &maxLength, &v[0]);

        String errorStr(v.begin(), v.end());
        Debug_Error("Failed to compile shader: '"  << m_filepath
                    << "': " << errorStr);
        glDeleteShader(m_idGL);
        return false;
    }

    return true;
}

GL::BindTarget Shader::GetGLBindTarget() const
{
    return GL::BindTarget::None;
}

const String& Shader::GetSourceCode() const
{
    return m_sourceCode;
}

const Path& Shader::GetFilepath() const
{
    return m_filepath;
}

Shader::Type Shader::GetType() const
{
    return m_type;
}

String Shader::ToString() const
{
    if (m_type == Type::Vertex)
    {
        return "Vertex Shader: '" + m_filepath + "'";
    }
    else
    {
        return "Fragment Shader: '" + m_filepath + "'";
    }
}
