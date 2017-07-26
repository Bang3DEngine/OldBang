#ifndef G_SHADERPROGRAM_H
#define G_SHADERPROGRAM_H

#include "Bang/Map.h"
#include "Bang/Color.h"
#include "Bang/Vector2.h"
#include "Bang/Vector3.h"
#include "Bang/Vector4.h"
#include "Bang/Matrix3.h"
#include "Bang/Matrix4.h"
#include "Bang/GLObject.h"

class G_Shader;
class G_Texture;
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

    bool SetInt(const String &name, int v) const;
    bool SetFloat(const String &name, float v) const;
    bool SetBool(const String &name, bool v) const;
    bool SetVec2(const String &name, const Vector2& v) const;
    bool SetVec3(const String &name, const Vector3& v) const;
    bool SetVec4(const String &name, const Vector4& v) const;
    bool SetColor(const String &name, const Color& c) const;
    bool SetMat3(const String &name, const Matrix3& m) const;
    bool SetMat4(const String &name, const Matrix4& m) const;
    bool SetTexture(const String &name, const G_Texture *texture) const;

    void Refresh();
    virtual void SetVertexShader(G_Shader *vertexShader);
    virtual void SetFragmentShader(G_Shader *fragmentShader);

    void SetVertexInputBinding(const String& inputName, uint location);
    void SetFragmentInputBinding(const String& inputName, uint location);

    G_Shader* GetVertexShader() const;
    G_Shader* GetFragmentShader() const;

    GLint GetUniformLocation(const String &name) const;
    GLint GetAttribLocation(const String &name) const;

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
