#include "Bang/G_Shader.h"

#include <sstream>
#include <fstream>
#include <iostream>

#include "Bang/File.h"
#include "Bang/Path.h"
#include "Bang/Array.h"
#include "Bang/Debug.h"

G_Shader::G_Shader() : G_Shader(GL::ShaderType::Vertex) { }

G_Shader::G_Shader(GL::ShaderType t)
    : m_sourceCode(""), m_type(t)
{
}

void G_Shader::Import(const Path& shaderFilepath)
{
    if (!shaderFilepath.Exists())
    {
        Debug_Error("Shader '" << shaderFilepath << "' does not exist.");
        return;
    }

    m_filepath = shaderFilepath;
    m_sourceCode = File::GetContents(m_filepath);
    PreprocessCode(&m_sourceCode);

    m_idGL = GL::CreateShader(m_type);

    GL::ShaderSource(m_idGL, m_sourceCode);
    if ( !GL::CompileShader(m_idGL) )
    {
        Debug_Error("Failed to compile shader: '"  << m_filepath
                    << "': " << GL::GetShaderErrorMsg(m_idGL));
        GL::DeleteShader(m_idGL);
        return;
    }
}

GL::BindTarget G_Shader::GetGLBindTarget() const
{
    return GL::BindTarget::None;
}

const String& G_Shader::GetSourceCode() const
{
    return m_sourceCode;
}

const Path& G_Shader::GetResourceFilepath() const
{
    return m_filepath;
}

GL::ShaderType G_Shader::GetType() const
{
    return m_type;
}

void G_Shader::PreprocessCode(String *shaderSrc) const
{
}
