#include "Bang/Shader.h"

#include "Bang/ShaderPreprocessor.h"

Shader::Shader(G_Shader::Type t) : G_Shader(t)
{
}

void Shader::PreprocessCode(String *shaderSrc) const
{
   ShaderPreprocessor::PreprocessCode(shaderSrc);
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
