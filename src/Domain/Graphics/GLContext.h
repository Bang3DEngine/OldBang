#ifndef GLCONTEXT_H
#define GLCONTEXT_H

#include "GL.h"
#include "List.h"
#include "Color.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "TypeMap.h"
#include "Texture.h"
#include "IRenderAgent.h"

class GLContext : public IRenderAgent
{
public:
    GLContext();
    virtual ~GLContext();

    virtual void ApplyToShaderProgram(ShaderProgram *sp) const;
    virtual void Apply() const;
    virtual void Reset();

    void SetWireframe(bool wireframe);
    void SetCullMode(GL::CullMode cullMode);
    void SetModelMatrix(const Matrix4 &model);
    void SetViewMatrix(const Matrix4 &view);
    void SetProjectionMatrix(const Matrix4 &projection);

    bool IsWireframe() const;
    GL::CullMode GetCullMode() const;
    const Matrix4 &GetModelMatrix() const;
    const Matrix4 &GetViewMatrix() const;
    const Matrix4 &GetProjectionMatrix() const;

private:
    bool m_wireframe = false;
    GL::CullMode m_cullMode = GL::CullMode::None;

    Matrix4 m_modelMatrix, m_viewMatrix, m_projectionMatrix;
};

#endif // GLCONTEXT_H
