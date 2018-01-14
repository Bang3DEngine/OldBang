#include "Bang/ShaderProgram.h"

#include "Bang/GL.h"
#include "Bang/Debug.h"
#include "Bang/Color.h"
#include "Bang/Shader.h"
#include "Bang/Vector2.h"
#include "Bang/Vector3.h"
#include "Bang/Vector4.h"
#include "Bang/Matrix3.h"
#include "Bang/Matrix4.h"
#include "Bang/XMLNode.h"
#include "Bang/Texture2D.h"
#include "Bang/Resources.h"
#include "Bang/GLUniforms.h"
#include "Bang/TextureUnitManager.h"

USING_NAMESPACE_BANG

ShaderProgram::ShaderProgram()
{
    m_idGL = GL::CreateProgram();
}

ShaderProgram::ShaderProgram(Shader *vShader, Shader *fShader) : ShaderProgram()
{
    Load(vShader, fShader);
}

ShaderProgram::ShaderProgram(const Path &vShaderPath,
                             const Path &fShaderPath) : ShaderProgram()
{
    Load(vShaderPath, fShaderPath);
}

ShaderProgram::~ShaderProgram()
{
    GL::DeleteProgram(m_idGL);
}

bool ShaderProgram::Load(const Path &vShaderPath, const Path &fShaderPath)
{
    RH<Shader> vShader = Resources::Load<Shader>(vShaderPath);
    RH<Shader> fShader = Resources::Load<Shader>(fShaderPath);
    return Load(vShader.Get(), fShader.Get());
}

bool ShaderProgram::Load(Shader* vShader, Shader* fShader)
{
    if(!vShader || !fShader ||
       (vShader && vShader == GetVertexShader()) ||
       (fShader && fShader == GetFragmentShader())) { return false; }

    SetVertexShader(vShader);
    SetFragmentShader(fShader);
    return Refresh();
}

bool ShaderProgram::Link()
{
    if (!GetVertexShader())
    {
        Debug_Error("Vertex shader not set. Can't link shader program.");
        return false;
    }

    if (!GetFragmentShader())
    {
        Debug_Error("Fragment shader not set. Can't link shader program.");
        return false;
    }

    if (m_idGL > 0) { GL::DeleteProgram(m_idGL); }
    m_isLinked = false;

    m_idGL = GL::CreateProgram();

    GL::AttachShader(m_idGL, GetVertexShader()->GetGLId());
    GL::AttachShader(m_idGL, GetFragmentShader()->GetGLId());

    if (!GL::LinkProgram(m_idGL))
    {
       Debug_Error("The shader program " << this << "( " <<
                   GetVertexShader()->GetResourceFilepath() << ", " <<
                   GetFragmentShader()->GetResourceFilepath() <<
                   ") did not link: " <<
                   GL::GetProgramLinkErrorMsg(m_idGL));
       GL::DeleteProgram(m_idGL); m_idGL = 0;
       return false;
    }

    m_isLinked = true;
    GLUniforms::BindAllUniformBuffersToShader(this);
    m_nameToLocationCache.clear(); // Invalidate cache
    m_namesToTexture.clear(); // Invalidate cache
    return true;
}

bool ShaderProgram::IsLinked() const
{
    return m_isLinked;
}

GL::BindTarget ShaderProgram::GetGLBindTarget() const
{
    return GL::BindTarget::ShaderProgram;
}

template<class T, class=TT_NOT_POINTER(T)>
bool SetShaderUniform(ShaderProgram *sp, const String &name, const T &v)
{
    ASSERT(GL::IsBound(sp));
    int location = sp->GetUniformLocation(name);
    if (location >= 0) { GL::Uniform(location, v); }
    return (location >= 0);
}

bool ShaderProgram::Set(const String &name, bool v)
{
    return SetShaderUniform(this, name, v);
}

bool ShaderProgram::Set(const String &name, float v)
{
    return SetShaderUniform(this, name, v);
}

bool ShaderProgram::Set(const String &name, const Color &v)
{
    return SetShaderUniform(this, name, v);
}

bool ShaderProgram::Set(const String &name, const Vector2 &v)
{
    return SetShaderUniform(this, name, v);
}

bool ShaderProgram::Set(const String &name, const Vector3 &v)
{
    return SetShaderUniform(this, name, v);
}

bool ShaderProgram::Set(const String &name, const Vector4 &v)
{
    return SetShaderUniform(this, name, v);
}

bool ShaderProgram::Set(const String &name, const Matrix3 &v)
{
    return SetShaderUniform(this, name, v);
}

bool ShaderProgram::Set(const String &name, const Matrix4 &v)
{
    return SetShaderUniform(this, name, v);
}

bool ShaderProgram::Set(const String &name, Texture2D *texture)
{
    bool needToAddTextureToMap = true;

    auto it = m_namesToTexture.find(name);
    if (it != m_namesToTexture.end())
    {
        if (texture != it->second)
        {
            needToAddTextureToMap = true;
            Texture2D *tex = it->second;
            if (tex) { tex->EventEmitter<IDestroyListener>::UnRegisterListener(this); }
        }
    }

    if (needToAddTextureToMap)
    {
        m_namesToTexture[name] = texture;
        if (texture) { texture->EventEmitter<IDestroyListener>::RegisterListener(this); }
        if (GL::IsBound(this)) { BindTextureToAvailableUnit(name, texture); }
    }
    return true;
}

bool ShaderProgram::Refresh()
{
    return Link();
}

bool ShaderProgram::SetVertexShader(Shader* vertexShader)
{
    if (vertexShader->GetType() != GL::ShaderType::Vertex)
    {
        Debug_Error("You are trying to set as vertex shader a "
                    "non-vertex shader");
        return false;
    }
    p_vshader.Set(vertexShader);
    return true;
}

bool ShaderProgram::SetFragmentShader(Shader* fragmentShader)
{
    if (fragmentShader->GetType() != GL::ShaderType::Fragment)
    {
        Debug_Error("You are trying to set as fragment shader a "
                    "non-fragment shader");
        return false;
    }
    p_fshader.Set(fragmentShader);
    return true;
}

Shader* ShaderProgram::GetVertexShader() const { return p_vshader.Get(); }
Shader* ShaderProgram::GetFragmentShader() const { return p_fshader.Get(); }

GLint ShaderProgram::GetUniformLocation(const String &name) const
{
    auto it = m_nameToLocationCache.find(name);
    if (it != m_nameToLocationCache.end()) { return it->second; }

    const GLuint location = GL::GetUniformLocation(m_idGL, name);
    m_nameToLocationCache[name] = location;
    return location;
}

bool ShaderProgram::BindTextureToAvailableUnit(const String &texName,
                                               Texture2D *texture) const
{
    int location = -1;
    if (texture)
    {
        location = GetUniformLocation(texName);
        if (location >= 0)
        {
            int texUnit = TextureUnitManager::BindTexture(texture);
            GL::Uniform(location, texUnit);
        }
    }
    return (location >= 0);
}


void ShaderProgram::Bind() const
{
    GL::Bind(this);
    UpdateTextureBindings();
}

void ShaderProgram::UnBind() const
{
    GL::UnBind(this);
}

void ShaderProgram::OnDestroyed(EventEmitter<IDestroyListener> *object)
{
    for (auto it = m_namesToTexture.begin(); it != m_namesToTexture.end(); )
    {
        Texture2D *destroyedTex = DCAST<Texture2D*>( SCAST<Object*>(object) );
        Texture2D *tex = it->second;
        if (tex == destroyedTex)
        {
            it = m_namesToTexture.erase(it);
            // Dont break, in case it has obj texture several times
        }
        else { ++it; }
    }
}

void ShaderProgram::UpdateTextureBindings() const
{
    for (const auto &nameTexPair : m_namesToTexture)
    {
        BindTextureToAvailableUnit(nameTexPair.first, nameTexPair.second);
    }
}
