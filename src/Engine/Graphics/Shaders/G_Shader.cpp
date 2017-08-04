#include "Bang/G_Shader.h"

#include <sstream>
#include <fstream>
#include <iostream>

#include "Bang/File.h"
#include "Bang/Path.h"
#include "Bang/Array.h"
#include "Bang/Debug.h"

G_Shader::G_Shader(G_Shader::Type t)
    : m_sourceCode(""), m_type(t)
{
}

bool G_Shader::LoadFromFile(const Path& filepath)
{
    if (!filepath.Exists())
    {
        Debug_Error("Shader '" << filepath << "' does not exist.");
        return false;
    }

    m_filepath = filepath;
    m_sourceCode = File::GetContents(m_filepath);
    PreprocessCode(&m_sourceCode);

    m_idGL = glCreateShader(GLint(m_type));

    const GLchar *source = (const GLchar*)(m_sourceCode.ToCString());
    GLint size = m_sourceCode.Size();
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

GL::BindTarget G_Shader::GetGLBindTarget() const
{
    return GL::BindTarget::None;
}

const String& G_Shader::GetSourceCode() const
{
    return m_sourceCode;
}

const Path& G_Shader::GetFilepath() const
{
    return m_filepath;
}

G_Shader::Type G_Shader::GetType() const
{
    return m_type;
}

void G_Shader::PreprocessCode(String *shaderSrc) const
{
}
