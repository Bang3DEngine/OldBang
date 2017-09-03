#include "Bang/G_ShaderProgram.h"

#include "Bang/Debug.h"
#include "Bang/Color.h"
#include "Bang/Vector2.h"
#include "Bang/Vector3.h"
#include "Bang/Vector4.h"
#include "Bang/Matrix3.h"
#include "Bang/Matrix4.h"
#include "Bang/G_Shader.h"
#include "Bang/G_Texture.h"
#include "Bang/G_TextureUnitManager.h"

G_ShaderProgram::G_ShaderProgram()
{
    m_idGL = GL::CreateProgram();
}

void G_ShaderProgram::Load(const Path &vshaderPath, const Path &fshaderPath)
{
    G_Shader *vs = new G_Shader(GL::ShaderType::Vertex);
    vs->Import(vshaderPath);
    SetVertexShader(vs);

    G_Shader *fs = new G_Shader(GL::ShaderType::Fragment);
    fs->Import(fshaderPath);
    SetFragmentShader(fs);
}


G_ShaderProgram::~G_ShaderProgram()
{
    GL::DeleteProgram(m_idGL);
}

bool G_ShaderProgram::Link()
{
    if (!p_vshader)
    {
        Debug_Error("Vertex shader not set. Can't link shader program.");
        return false;
    }

    if (!p_fshader)
    {
        Debug_Error("Fragment shader not set. Can't link shader program.");
        return false;
    }

    if (m_idGL > 0) { GL::DeleteProgram(m_idGL); }
    m_idGL = GL::CreateProgram();

    GL::AttachShader(m_idGL, p_vshader->GetGLId());
    GL::AttachShader(m_idGL, p_fshader->GetGLId());

    OnPreLink();
    if (!GL::LinkProgram(m_idGL))
    {
       Debug_Error("The shader program " << this << " did not link: " <<
                   GL::GetProgramLinkErrorMsg(m_idGL));
       GL::DeleteProgram(m_idGL); m_idGL = 0;
       return false;
    }

    m_nameToLocationCache.Clear(); // Invalidate cache
    return true;
}

GL::BindTarget G_ShaderProgram::GetGLBindTarget() const
{
    return GL::BindTarget::ShaderProgram;
}

bool G_ShaderProgram::Set(const String &name, const G_Texture *texture) const
{
    bool uniformIsUsed = BindTextureToAvailableUnit(name, texture);
    if (uniformIsUsed)
    {
        m_namesToTexture[name] = texture;
    }
    return uniformIsUsed;
}

void G_ShaderProgram::Refresh()
{
    ENSURE(p_vshader && p_fshader);
    Link();
}

void G_ShaderProgram::SetVertexShader(G_Shader *vertexShader)
{
    if (vertexShader->GetType() != GL::ShaderType::Vertex)
    {
        Debug_Error("You are trying to set as vertex shader a non-vertex shader");
    }
    p_vshader = vertexShader;
    Refresh();
}

void G_ShaderProgram::SetFragmentShader(G_Shader *fragmentShader)
{
    if (fragmentShader->GetType() != GL::ShaderType::Fragment)
    {
        Debug_Error("You are trying to set as fragment shader a non-fragment shader");
    }
    p_fshader = fragmentShader;
    Refresh();
}

void G_ShaderProgram::SetVertexInputBinding(const String &attribName,
                                            uint location)
{
    GL::BindAttribLocation(m_idGL, location, attribName);
}

void G_ShaderProgram::SetFragmentInputBinding(const String &fragDataName,
                                              uint location)
{
    GL::BindFragDataLocation(m_idGL, location, fragDataName);
}

G_Shader *G_ShaderProgram::GetVertexShader() const
{
    return p_vshader;
}

G_Shader *G_ShaderProgram::GetFragmentShader() const
{
    return p_fshader;
}

GLint G_ShaderProgram::GetUniformLocation(const String &name) const
{
    auto it = m_nameToLocationCache.Find(name);
    if (it != m_nameToLocationCache.End()) { return it->second; }

    const GLuint location = GL::GetUniformLocation(m_idGL, name);
    m_nameToLocationCache[name] = location;
    return location;
}

bool G_ShaderProgram::BindTextureToAvailableUnit(const String &texName,
                                                 const G_Texture *texture) const
{
    int location = -1;
    if (texture)
    {
        location = GetUniformLocation(texName);
        if (location >= 0)
        {
            int texUnit = G_TextureUnitManager::BindTexture(texture);
            GL::Uniform(location, texUnit);
        }
    }
    return (location >= 0);
}

void G_ShaderProgram::Bind() const
{
    GL::Bind(this);
    UpdateTextureBindings();
}

void G_ShaderProgram::UnBind() const
{
    GL::UnBind(this);
}

void G_ShaderProgram::UpdateTextureBindings() const
{
    for (auto it = m_namesToTexture.Begin(); it != m_namesToTexture.End(); ++it)
    {
        BindTextureToAvailableUnit(it->first, it->second);
    }
}

void G_ShaderProgram::OnPreLink()
{
}
