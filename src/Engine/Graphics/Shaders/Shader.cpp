#include "Bang/Shader.h"

#include <sstream>
#include <fstream>
#include <iostream>

#include "Bang/GL.h"
#include "Bang/File.h"
#include "Bang/Path.h"
#include "Bang/Array.h"
#include "Bang/Debug.h"
#include "Bang/ShaderPreprocessor.h"

USING_NAMESPACE_BANG

Shader::Shader(GL_ShaderType t) : m_type(t)
{
}

Shader::Shader() : Shader(GL_ShaderType::Vertex)
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
                    << "': " << std::endl << GL::GetShaderErrorMsg(m_idGL));
        GL::DeleteShader(m_idGL);
        return;
    }
}

void Shader::RetrieveType(const Path &shaderPath)
{
    if (shaderPath.GetExtension().Contains("vert"))
    {
        m_type = GL_ShaderType::Vertex;
    }
    else { m_type = GL_ShaderType::Fragment; }
}

GL_BindTarget Shader::GetGLBindTarget() const
{
    return GL_BindTarget::None;
}

const String& Shader::GetSourceCode() const
{
    return m_sourceCode;
}

GL_ShaderType Shader::GetType() const
{
    return m_type;
}
