#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "Bang.h"

#include "Matrix4.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Texture.h"
#include "IGLBindable.h"


#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Color.h"

class ShaderProgram : public IGLIdable
                     ,public IGLBindable
                     ,public IToString
{
private:
    Shader *m_vshader;
    Shader *m_fshader;

    mutable std::map<String, const Texture*> m_namesToTextures;

public:
    ShaderProgram();
    ShaderProgram(const String &vshaderPath,
                  const String &fshaderPath);
    virtual ~ShaderProgram();

    void BindVertexShader(Shader *vshader);
    void BindFragmentShader(Shader *fshader);

    bool Link();

    void Bind() const override;
    void UnBind() const override;

    bool SetUniformFloat   (const String &name, float v,                bool warn = true) const;
    bool SetUniformVec2    (const String &name, const Vector2& v,       bool warn = true) const;
    bool SetUniformVec3    (const String &name, const Vector3& v,       bool warn = true) const;
    bool SetUniformVec4    (const String &name, const Vector4& v,       bool warn = true) const;
    bool SetUniformColor   (const String &name, const Color& c,         bool warn = true) const;
    bool SetUniformMat4    (const String &name, const Matrix4& m,       bool warn = true) const;
    bool SetUniformTexture (const String &name, const Texture *texture, bool warn = true) const;

    Shader* GetVertexShader() const;
    Shader* GetFragmentShader() const;

    GLint GetUniformLocation(const String &name) const;
    GLint GetAttribLocation(const String &name) const;

    const String ToString() const override;
};

#endif // SHADERPROGRAM_H
