#include "Bang/Shader.h"

#include <sstream>
#include <fstream>
#include <iostream>

#include "Bang/File.h"
#include "Bang/Path.h"
#include "Bang/Array.h"
#include "Bang/Debug.h"
#include "Bang/ShaderPreprocessor.h"

USING_NAMESPACE_BANG

Shader::Shader() : Shader(GL::ShaderType::Vertex)
{
}

Shader::Shader(GL::ShaderType t) : m_type(t)
{
}

void Shader::Import(const Path& shaderFilepath)
{
    if (!shaderFilepath.Exists())
    {
        Debug_Error("Shader '" << shaderFilepath << "' does not exist.");
        return;
    }

    RetrieveType(shaderFilepath);
    m_sourceCode = File::GetContents(shaderFilepath);
    ShaderPreprocessor::PreprocessCode(&m_sourceCode);

    m_idGL = GL::CreateShader(m_type);

    GL::ShaderSource(m_idGL, m_sourceCode);
    if ( !GL::CompileShader(m_idGL) )
    {
        Debug_Error("Failed to compile shader: '"  << shaderFilepath
                    << "': " << GL::GetShaderErrorMsg(m_idGL));
        GL::DeleteShader(m_idGL);
        return;
    }
}

void Shader::RetrieveType(const Path &shaderPath)
{
    if (shaderPath.GetExtension().Contains("vert"))
    {
        m_type = GL::ShaderType::Vertex;
    }
    else { m_type = GL::ShaderType::Fragment; }
}

GL::BindTarget Shader::GetGLBindTarget() const
{
    return GL::BindTarget::None;
}

const String& Shader::GetSourceCode() const
{
    return m_sourceCode;
}

GL::ShaderType Shader::GetType() const
{
    return m_type;
}
