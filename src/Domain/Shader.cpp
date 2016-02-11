#include "Shader.h"

Shader::Shader(Shader::Type t) : sourceCode(""), filepath(""), type(t)
{
}

Shader::Shader(Shader::Type t, const std::string &shaderPath) : Shader(t)
{
    LoadFromFile(shaderPath);
}

bool Shader::LoadFromFile(const std::string& filepath)
{
    this->filepath = filepath;

    std::ifstream f;
    f.open(filepath);
    if(not f.is_open())
    {
        Logger_Error("Error opening file '" + filepath + "'");
        return false;
    }

    std::stringstream ss;
    ss << f.rdbuf();
    sourceCode = ss.str();


    idgl = glCreateShader(type);

    const GLchar *source = (const GLchar*)(sourceCode.c_str());
    GLint size = sourceCode.length();
    glShaderSource(idgl, 1, &source, &size);
    glCompileShader(idgl);

    GLint ok;
    glGetShaderiv(idgl, GL_COMPILE_STATUS, &ok);
    if (not ok)
    {
        GLint maxLength = 0;
        glGetShaderiv(idgl, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> v(maxLength);
        glGetShaderInfoLog(idgl, maxLength, &maxLength, &v[0]);

        std::string errorStr(v.begin(), v.end());
        Logger_Error("Failed to compile shader: '" + filepath + "': " + errorStr);
        glDeleteShader(idgl);
        return false;
    }

    return true;
}

const std::string& Shader::GetSourceCode() const
{
    return sourceCode;
}

const std::string& Shader::GetFilepath() const
{
    return filepath;
}

Shader::Type Shader::GetType() const
{
    return type;
}

const std::string Shader::ToString() const
{
    if(type == Type::Vertex) return "Vertex Shader: '" + filepath + "'";
    return "Fragment Shader: '" + filepath + "'";
}
