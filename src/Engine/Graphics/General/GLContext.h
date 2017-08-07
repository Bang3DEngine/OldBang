#ifndef GLCONTEXT_H
#define GLCONTEXT_H

#include <stack>

#include "Bang/GL.h"
#include "Bang/Map.h"
#include "Bang/Matrix4.h"

FORWARD class G_VAO;

class GLContext
{
public:
    GLContext();
    virtual ~GLContext();

    virtual void ApplyToShaderProgram(G_ShaderProgram *sp) const;

    void Render(const G_VAO* vao, GL::RenderPrimitive renderMode,
                int elementsCount, int startIndex) const;

    void SetViewProjMode(GL::ViewProjMode mode);
    void SetWriteDepth(bool writeDepth);
    void SetTestDepth(bool testDepth);
    void SetWireframe(bool wireframe);
    void SetCullMode(GL::CullMode cullMode);
    void SetModelMatrix(const Matrix4 &model);
    void SetViewMatrix(const Matrix4 &view);
    void SetProjectionMatrix(const Matrix4 &projection);
    void SetZNearFar(float zNear, float zFar);

    bool IsWriteDepth() const;
    bool IsTestDepth() const;
    bool IsWireframe() const;
    bool IsBound(GL::BindTarget bindTarget, GLId glId) const;

    GL::CullMode GetCullMode() const;
    const Matrix4 &GetModelMatrix() const;
    const Matrix4 &GetViewMatrix() const;
    const Matrix4 &GetProjectionMatrix() const;

    GLId GetBoundId(GL::BindTarget bindTarget) const;

private:
    bool m_writeDepth = true, m_testDepth = true, m_wireframe = false;
    GL::CullMode m_cullMode = GL::CullMode::None;
    Map<GL::BindTarget, std::stack<GLId> > m_glBoundIds;

    GL::ViewProjMode m_viewProjMode = GL::ViewProjMode::UseBoth;
    Matrix4 m_modelMatrix, m_viewMatrix, m_projectionMatrix;
    float m_zNear, m_zFar;

    void OnBind(GL::BindTarget bindTarget, GLId glId);
    void OnUnBind(GL::BindTarget bindTarget);

    friend class GL;
};

#endif // GLCONTEXT_H
