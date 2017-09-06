#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "Bang/Map.h"
#include "Bang/Asset.h"
#include "Bang/GLObject.h"

NAMESPACE_BANG_BEGIN

FORWARD class Shader;
FORWARD class Texture;

class ShaderProgram : public GLObject,
                      public Asset
{
    ASSET(ShaderProgram)

public:
    enum InputType
    {
        GBuffer,
        PostProcess,
        Other
    };

    ShaderProgram();
    virtual ~ShaderProgram();

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

    bool Set(const String &name, const Texture *texture) const;

    void Refresh();
    void SetInputType(InputType type);
    void SetVertexShader(Shader *vertexShader);
    void SetFragmentShader(Shader *fragmentShader);

    void SetVertexInputBinding(const String& vertexInputName, uint location);
    void SetFragmentInputBinding(const String& fragInputName, uint location);

    InputType GetInputType() const;
    Shader* GetVertexShader() const;
    Shader* GetFragmentShader() const;

    GLint GetUniformLocation(const String &name) const;

    void RetrieveType(const Path &vshaderPath, const Path &fshaderPath);

    // Resource
    void Import(const Path &shaderProgramFilepath) override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

private:
    Shader *p_vshader = nullptr;
    Shader *p_fshader = nullptr;

    InputType m_inputType = InputType::GBuffer;

    mutable Map<String, GLuint> m_nameToLocationCache;
    mutable Map<String, const Texture*> m_namesToTexture;

    bool BindTextureToAvailableUnit(const String &texName,
                                    const Texture *texture) const;
    void UpdateTextureBindings() const;
};

NAMESPACE_BANG_END

#endif // SHADERPROGRAM_H
