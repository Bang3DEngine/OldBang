#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "Map.h"
#include "Color.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"

#include "GLObject.h"
#include "IToString.h"
#include "IRenderAgent.h"

class Shader;
class Texture;
class ShaderProgram : public GLObject
                     ,public IToString
                     ,public IRenderAgent
{

public:
    ShaderProgram();
    ShaderProgram(const String &vshaderPath,
                  const String &fshaderPath);
    virtual ~ShaderProgram();

    void BindVertexShader(Shader *vshader);
    void BindFragmentShader(Shader *fshader);

    bool Link();

    virtual void OnRenderingStarts(GameObject *go, ShaderProgram *sp) override;
    virtual void OnRenderingEnds  (GameObject *go, ShaderProgram *sp) override;
    GL::BindTarget GetGLBindTarget() const override;
    void Bind() const override;
    void UnBind() const override;

    bool SetFloat(const String &name, float v) const;
    bool SetVec2(const String &name, const Vector2& v) const;
    bool SetVec3(const String &name, const Vector3& v) const;
    bool SetVec4(const String &name, const Vector4& v) const;
    bool SetColor(const String &name, const Color& c) const;
    bool SetMat4(const String &name, const Matrix4& m) const;
    bool SetTexture(const String &name, const Texture *texture) const;

    Shader* GetVertexShader() const;
    Shader* GetFragmentShader() const;

    GLint GetUniformLocation(const String &name) const;
    GLint GetAttribLocation(const String &name) const;

    String ToString() const override;

private:
    Shader *m_vshader = nullptr;
    Shader *m_fshader = nullptr;

    mutable Map<String, const Texture*> m_names_To_Texture;
    void UpdateTextureBindings() const;
};

#endif // SHADERPROGRAM_H
