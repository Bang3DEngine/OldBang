#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "Bang/Map.h"
#include "Bang/GLObject.h"

NAMESPACE_BANG_BEGIN

FORWARD class Shader;
FORWARD class Texture;

class ShaderProgram : public GLObject
{
public:
    enum InputType
    {
        GBuffer,
        PostProcess,
        Other
    };

    bool Load(const Path &vshaderPath, const Path &fshaderPath);
    bool Load(Shader *vShader, Shader *fShader);

    bool Link();
    bool IsLinked() const;

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

    bool Set(const String &name, const Texture *texture) const;

    bool Refresh();
    void SetInputType(InputType type);
    bool SetVertexShader(Shader *vertexShader);
    bool SetFragmentShader(Shader *fragmentShader);

    void SetVertexInputBinding(const String& vertexInputName, uint location);
    void SetFragmentInputBinding(const String& fragInputName, uint location);

    InputType GetInputType() const;
    Shader* GetVertexShader() const;
    Shader* GetFragmentShader() const;

    GLint GetUniformLocation(const String &name) const;

    void RetrieveType(const Path &vshaderPath, const Path &fshaderPath);

private:
    ShaderProgram();
    virtual ~ShaderProgram();

    bool m_isLinked = false;
    Shader *p_vshader = nullptr;
    Shader *p_fshader = nullptr;

    InputType m_inputType = InputType::GBuffer;

    mutable Map<String, GLuint> m_nameToLocationCache;
    mutable Map<String, const Texture*> m_namesToTexture;

    bool BindTextureToAvailableUnit(const String &texName,
                                    const Texture *texture) const;
    void UpdateTextureBindings() const;

    friend class ShaderProgramFactory;
};

NAMESPACE_BANG_END

#endif // SHADERPROGRAM_H
