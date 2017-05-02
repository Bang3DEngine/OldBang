#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "Bang/Map.h"
#include "Bang/UMap.h"
#include "Bang/Color.h"
#include "Bang/Asset.h"
#include "Bang/Vector2.h"
#include "Bang/Vector3.h"
#include "Bang/Vector4.h"
#include "Bang/Matrix4.h"
#include "Bang/GLObject.h"
#include "Bang/IToString.h"

class Shader;
class Texture;
class ShaderProgram : public Asset,
                      public GLObject
{
    OBJECT(ShaderProgram)
    ASSET_ICON(ShaderProgram, "Icons/BehaviourIcon.png")

public:
    enum Type
    {
        GBuffer,
        PostProcess,
        SelectionFramebuffer,
        Other
    };

    ShaderProgram();
    ShaderProgram(const Path &vshaderPath, const Path &fshaderPath);
    ShaderProgram(Type type, const Path &vshaderPath, const Path &fshaderPath);
    virtual ~ShaderProgram();

    static String GetFileExtensionStatic();
    virtual String GetFileExtension() const override;

    bool Link();

    void Bind() const override;
    void UnBind() const override;
    GL::BindTarget GetGLBindTarget() const override;

    bool SetInt(const String &name, int v) const;
    bool SetFloat(const String &name, float v) const;
    bool SetBool(const String &name, bool v) const;
    bool SetVec2(const String &name, const Vector2& v) const;
    bool SetVec3(const String &name, const Vector3& v) const;
    bool SetVec4(const String &name, const Vector4& v) const;
    bool SetColor(const String &name, const Color& c) const;
    bool SetMat3(const String &name, const glm::mat3& m) const;
    bool SetMat4(const String &name, const Matrix4& m) const;
    bool SetTexture(const String &name, const Texture *texture) const;

    void Refresh();
    void SetType(Type type);
    void SetVertexShader(Shader *vertexShader);
    void SetFragmentShader(Shader *fragmentShader);

    void SetVertexInputBinding(const String& inputName, uint location);
    void SetFragmentInputBinding(const String& inputName, uint location);

    Type GetType() const;

    Shader* GetVertexShader() const;
    Shader* GetFragmentShader() const;

    GLint GetUniformLocation(const String &name) const;
    GLint GetAttribLocation(const String &name) const;

    String ToString() const override;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

private:
    Type m_type       = Type::GBuffer;
    Shader *p_vshader = nullptr;
    Shader *p_fshader = nullptr;

    void Init(Type type, const Path &vshaderPath, const Path &fshaderPath);

    mutable Map<String, GLuint> m_nameToLocationCache;
    mutable Map<String, const Texture*> m_namesToTexture;

    bool BindTextureToAvailableUnit(const String &texName,
                                    const Texture *texture) const;
    void UpdateTextureBindings() const;
};

#endif // SHADERPROGRAM_H
