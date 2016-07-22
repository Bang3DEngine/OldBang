#include "Shader.h"

Shader::Shader(Shader::Type t) : m_sourceCode(""), m_filepath(""), m_type(t)
{
}

Shader::Shader(Shader::Type t, const std::string &shaderPath) : Shader(t)
{
    LoadFromFile(shaderPath);
}

bool Shader::LoadFromFile(const std::string& filepath)
{
    this->m_filepath = filepath;

    std::ifstream f;
    f.open(filepath);
    if (!f.is_open())
    {
        Logger_Error("Could not open shader file '" + filepath + "'. Does it exist / is reachable?");
        return false;
    }

    std::stringstream ss;
    ss << f.rdbuf();
    m_sourceCode = ss.str();

    ShaderPreprocessor::PreprocessShaderCode(&m_sourceCode);

    m_idGL = glCreateShader(m_type);

    const GLchar *source = (const GLchar*)(m_sourceCode.c_str());
    GLint size = m_sourceCode.length();
    glShaderSource(m_idGL, 1, &source, &size);
    glCompileShader(m_idGL);

    GLint ok;
    glGetShaderiv(m_idGL, GL_COMPILE_STATUS, &ok);
    if (!ok)
    {
        GLint maxLength = 0;
        glGetShaderiv(m_idGL, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> v(maxLength);
        glGetShaderInfoLog(m_idGL, maxLength, &maxLength, &v[0]);

        std::string errorStr(v.begin(), v.end());
        Logger_Error("Failed to compile shader: '" + filepath + "': " + errorStr);
        glDeleteShader(m_idGL);
        return false;
    }

    return true;
}

const std::string& Shader::GetSourceCode() const
{
    return m_sourceCode;
}

const std::string& Shader::GetFilepath() const
{
    return m_filepath;
}

Shader::Type Shader::GetType() const
{
    return m_type;
}

const std::string Shader::ToString() const
{
    if (m_type == Type::Vertex) return "Vertex Shader: '" + m_filepath + "'";
    return "Fragment Shader: '" + m_filepath + "'";
}
