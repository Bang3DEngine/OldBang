#include "Shader.h"

#include <sstream>
#include <fstream>
#include <iostream>

#include "Debug.h"
#include "IO.h"
#include "ShaderPreprocessor.h"

Shader::Shader(Shader::Type t)
    : m_sourceCode(""), m_filepath(""), m_type(t)
{
}

Shader::Shader(Shader::Type t, const String &shaderPath) : Shader(t)
{
    LoadFromFile(shaderPath);
}

bool Shader::LoadFromFile(const String& filepath)
{
    if (!IO::ExistsFile(filepath))
    {
        Debug_Error("Shader '" << filepath << "' does not exist.");
        return false;
    }

    // Mini cache to avoid recompiling
    static Map<String, GLuint> s_cacheFilepathToIdGL;

    m_filepath = filepath;

    if (s_cacheFilepathToIdGL.ContainsKey(m_filepath))
    {
        m_idGL = s_cacheFilepathToIdGL.Get(m_filepath);
    }
    else
    {
        m_sourceCode = IO::GetFileContents(m_filepath);
        if (m_filepath.Empty())
        {
            Debug_Error("Could not open shader file '" + filepath +
                        "'. Does it exist / is reachable?");
            return false;
        }
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
            Debug_Error("Failed to compile shader: '"
                        + m_filepath + "': " + errorStr);
            glDeleteShader(m_idGL);
            return false;
        }

        s_cacheFilepathToIdGL.Set(m_filepath, m_idGL);
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

const String& Shader::GetFilepath() const
{
    return m_filepath;
}

Shader::Type Shader::GetType() const
{
    return m_type;
}

String Shader::ToString() const
{
    if (m_type == Type::Vertex) return "Vertex Shader: '" + m_filepath + "'";
    return "Fragment Shader: '" + m_filepath + "'";
}
