#include "Shader.h"

Shader::Shader(Type t) : type(t)
{
}

Shader::Shader(Shader::Type t, const std::string &filepath) : Shader(t)
{
    LoadFromFile(filepath);
}

bool Shader::LoadFromFile(const std::string& filepath)
{
    std::ifstream f;
    try
    {
        f.open(filepath);
    }
    catch(std::ios_base::failure &e)
    {
        Logger::Error("Error opening file '" + filepath + "'");
    }

    std::stringstream ss;
    ss << f.rdbuf();
    sourceCode = ss.str();


    std::cout << "xzczxc" << std::endl;
    id = glCreateShader(type);
    std::cout << "vcbb" << std::endl;

    GLchar *source = (GLchar*)(sourceCode.c_str());
    GLint size = sourceCode.length();
    glShaderSource(id, 1, &source, &size);
    glCompileShader(id);

    GLint ok;
    glGetShaderiv(id, GL_COMPILE_STATUS, &ok);
    if (not ok)
    {
        Logger::Error("Failed to compile shader: '" + filepath + "'");
        glDeleteShader(id);
        return 0;
    }

    return true;
}

const std::string Shader::ToString() const
{
    if(type == Type::Vertex) return "Vertex Shader";
    return "Fragment Shader";
}
