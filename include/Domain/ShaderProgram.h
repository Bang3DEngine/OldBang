#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "Shader.h"
#include "GLBindable.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class ShaderProgram : public GLIdable, public GLBindable
{
private:
    Shader *vshader, *fshader;

public:
    ShaderProgram();
    virtual ~ShaderProgram();

    void BindVertexShader(Shader *vshader);
    void BindFragmentShader(Shader *fshader);

    bool Link();

    void Bind() const override;
    void UnBind() const override;

    bool SetUniformFloat(const std::string &name, float v) const;
    bool SetUniformVec2 (const std::string &name, const glm::vec2& v) const;
    bool SetUniformVec3 (const std::string &name, const glm::vec3& v) const;
    bool SetUniformVec4 (const std::string &name, const glm::vec4& v) const;
    bool SetUniformMat4 (const std::string &name, const glm::mat4& m) const;

    Shader* GetVertexShader() const { return vshader; }
    Shader* GetFragmentShader() const { return fshader; }
};

#endif // SHADERPROGRAM_H
