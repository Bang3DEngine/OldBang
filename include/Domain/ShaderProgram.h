#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "Shader.h"
#include "GLBindable.h"
#include "glm/glm.hpp"

class ShaderProgram : public GLIdable, public GLBindable
{
private:
    GLint lastBoundShaderProgramId;
    Shader *vshader, *fshader;

public:
    ShaderProgram();
    virtual ~ShaderProgram();

    void BindVertexShader(Shader *vshader);
    void BindFragmentShader(Shader *fshader);

    bool Link();

    void Bind() const override;
    void UnBind() const override;

    void SetUniformFloat(const std::string &name, float v);
    void SetUniformVec2 (const std::string &name, const glm::vec2& v);
    void SetUniformVec3 (const std::string &name, const glm::vec3& v);
    void SetUniformVec4 (const std::string &name, const glm::vec4& v);
    void SetUniformMat4 (const std::string &name, const glm::mat4& m);

    Shader* GetVertexShader() const { return vshader; }
    Shader* GetFragmentShader() const { return fshader; }
};

#endif // SHADERPROGRAM_H
