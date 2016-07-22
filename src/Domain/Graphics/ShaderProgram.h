#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "Bang.h"

#include "Matrix4.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Texture.h"
#include "IGLBindable.h"

#include <string>


class ShaderProgram : public IGLIdable
                     ,public IGLBindable
                     ,public IToString
{
private:
    Shader *m_vshader;
    Shader *m_fshader;

    mutable std::map<std::string, Texture*> m_namesToTextures;

public:
    ShaderProgram();
    ShaderProgram(const std::string &vshaderPath,
                  const std::string &fshaderPath);
    virtual ~ShaderProgram();

    void BindVertexShader(Shader *vshader);
    void BindFragmentShader(Shader *fshader);

    bool Link();

    void Bind() const override;
    void UnBind() const override;

    bool SetUniformFloat   (const std::string &name, float v,                bool warn = true) const;
    bool SetUniformVec2    (const std::string &name, const glm::vec2& v,     bool warn = true) const;
    bool SetUniformVec3    (const std::string &name, const Vector3& v,       bool warn = true) const;
    bool SetUniformVec4    (const std::string &name, const glm::vec4& v,     bool warn = true) const;
    bool SetUniformMat4    (const std::string &name, const Matrix4& m,       bool warn = true) const;
    bool SetUniformTexture (const std::string &name, Texture *texture,       bool warn = true) const;

    Shader* GetVertexShader() const;
    Shader* GetFragmentShader() const;

    GLint GetUniformLocation(const std::string &name) const;
    GLint GetAttribLocation(const std::string &name) const;

    const std::string ToString() const override;
};

#endif // SHADERPROGRAM_H
