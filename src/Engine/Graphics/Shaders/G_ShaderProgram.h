#ifndef G_SHADERPROGRAM_H
#define G_SHADERPROGRAM_H

#include "Bang/Map.h"
#include "Bang/GLObject.h"

FORWARD   class G_Shader;
FORWARD   class G_Texture;

class G_ShaderProgram : public GLObject
{
public:
    G_ShaderProgram();
    virtual ~G_ShaderProgram();

    virtual void Load(const Path &vshaderPath, const Path &fshaderPath);

    bool Link();

    void Bind() const override;
    void UnBind() const override;
    GL::BindTarget GetGLBindTarget() const override;

    template<class T, class=TT_NOT_POINTER(T)>
    bool Set(const String &name, const T &v) const
    {
        ASSERT(GL::IsBound(this));
        int location = GetUniformLocation(name);
        if (location >= 0) { GL::Uniform(location, v); }
        return (location >= 0);
    }

    bool Set(const String &name, const G_Texture *texture) const;

    void Refresh();
    virtual void SetVertexShader(G_Shader *vertexShader);
    virtual void SetFragmentShader(G_Shader *fragmentShader);

    void SetVertexInputBinding(const String& vertexInputName, uint location);
    void SetFragmentInputBinding(const String& fragInputName, uint location);

    G_Shader* GetVertexShader() const;
    G_Shader* GetFragmentShader() const;

    GLint GetUniformLocation(const String &name) const;

protected:
    G_Shader *p_vshader = nullptr;
    G_Shader *p_fshader = nullptr;

    mutable Map<String, GLuint> m_nameToLocationCache;
    mutable Map<String, const G_Texture*> m_namesToTexture;

    bool BindTextureToAvailableUnit(const String &texName,
                                    const G_Texture *texture) const;
    void UpdateTextureBindings() const;

    virtual void OnPreLink();
};


#endif // G_SHADERPROGRAM_H
