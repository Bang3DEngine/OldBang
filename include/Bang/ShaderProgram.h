#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <unordered_map>

#include "Bang/Map.h"
#include "Bang/GLObject.h"
#include "Bang/Resources.h"
#include "Bang/IDestroyListener.h"

NAMESPACE_BANG_BEGIN

FORWARD class Shader;
FORWARD class Texture2D;

class ShaderProgram : public GLObject,
                      public Resource,
                      public IDestroyListener
{
    RESOURCE(ShaderProgram)

public:
    bool Load(const Path &vshaderPath, const Path &fshaderPath);
    bool Load(Shader* vShader, Shader* fShader);

    bool Link();
    bool IsLinked() const;

    void Bind() const override;
    void UnBind() const override;
    GL::BindTarget GetGLBindTarget() const override;

    bool Set(const String &name, int v, bool warn = true);
    bool Set(const String &name, bool v, bool warn = true);
    bool Set(const String &name, float v, bool warn = true);
    bool Set(const String &name, const Color& v, bool warn = true);
    bool Set(const String &name, const Vector2& v, bool warn = true);
    bool Set(const String &name, const Vector3& v, bool warn = true);
    bool Set(const String &name, const Vector4& v, bool warn = true);
    bool Set(const String &name, const Matrix3& v, bool warn = true);
    bool Set(const String &name, const Matrix4& v, bool warn = true);
    bool Set(const String &name, Texture2D *texture, bool warn = true);

    bool Refresh();
    bool SetVertexShader(Shader* vertexShader);
    bool SetFragmentShader(Shader* fragmentShader);

    Shader* GetVertexShader() const;
    Shader* GetFragmentShader() const;

    GLint GetUniformLocation(const String &name) const;

    // Resource
    void Import(const Path &resourceFilepath) override;

private:
    RH<Shader> p_vshader;
    RH<Shader> p_fshader;
    bool m_isLinked = false;

    std::unordered_map<String, int> m_uniformCacheInt;
    std::unordered_map<String, bool> m_uniformCacheBool;
    std::unordered_map<String, float> m_uniformCacheFloat;
    std::unordered_map<String, Color> m_uniformCacheColor;
    std::unordered_map<String, Vector2> m_uniformCacheVector2;
    std::unordered_map<String, Vector3> m_uniformCacheVector3;
    std::unordered_map<String, Vector4> m_uniformCacheVector4;
    std::unordered_map<String, Matrix3> m_uniformCacheMatrix3;
    std::unordered_map<String, Matrix4> m_uniformCacheMatrix4;
    mutable std::unordered_map<String, GLuint> m_nameToLocationCache;
    mutable std::unordered_map<String, Texture2D*> m_namesToTexture;

    ShaderProgram();
    ShaderProgram(Shader *vShader, Shader *fShader);
    ShaderProgram(const Path& vShaderPath,
                  const Path& fShaderPath);
    virtual ~ShaderProgram();

    bool BindTextureToAvailableUnit(const String &texName, Texture2D *texture) const;
    void UpdateTextureBindings() const;

    // IDestroyListener
    void OnDestroyed(EventEmitter<IDestroyListener> *object) override;
};

NAMESPACE_BANG_END

#endif // SHADERPROGRAM_H
