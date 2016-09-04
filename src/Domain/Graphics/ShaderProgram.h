#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "Bang.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Map.h"
#include "Color.h"
#include "Shader.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Texture.h"
#include "IGLBindable.h"

class ShaderProgram : public IGLIdable
                     ,public IGLBindable
                     ,public IToString
{
private:
    Shader *m_vshader;
    Shader *m_fshader;

    mutable Map<String, const Texture*> m_names_To_Textures;

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

    bool SetUniformFloat   (const String &name, float v,                bool warn = false) const;
    bool SetUniformVec2    (const String &name, const Vector2& v,       bool warn = false) const;
    bool SetUniformVec3    (const String &name, const Vector3& v,       bool warn = false) const;
    bool SetUniformVec4    (const String &name, const Vector4& v,       bool warn = false) const;
    bool SetUniformColor   (const String &name, const Color& c,         bool warn = false) const;
    bool SetUniformMat4    (const String &name, const Matrix4& m,       bool warn = false) const;
    bool SetUniformTexture (const String &name, const Texture *texture, bool warn = false) const;

    Shader* GetVertexShader() const;
    Shader* GetFragmentShader() const;

    GLint GetUniformLocation(const String &name) const;
    GLint GetAttribLocation(const String &name) const;

    const String ToString() const override;
};

#endif // SHADERPROGRAM_H
