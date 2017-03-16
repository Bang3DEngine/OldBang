#include "GLContext.h"

#include "GL/gl.h"
#include "GL/glew.h"

#include "VAO.h"
#include "Debug.h"
#include "Scene.h"
#include "Camera.h"
#include "Transform.h"
#include "SceneManager.h"
#include "ShaderProgram.h"

void GLContext::ApplyToShaderProgram(ShaderProgram *sp) const
{
    sp->Bind();

    Camera *cam = SceneManager::GetActiveScene()->GetCamera();
    sp->SetVec3("B_CameraPositionWorld",
                cam->gameObject->transform->GetPosition());

    sp->SetMat4("B_Model",    m_modelMatrix);
    sp->SetMat4("B_ModelInv", m_modelMatrix.Inversed());

    glm::mat3 normalMatrix = glm::mat3(m_modelMatrix.c0.xyz(),
                                       m_modelMatrix.c1.xyz(),
                                       m_modelMatrix.c2.xyz());
    normalMatrix = glm::inverse( glm::transpose(normalMatrix) );
    // Matrix4 normalMatrix = m_modelMatrix.Inversed().Transposed();
    sp->SetMat3("B_Normal",    normalMatrix);
    sp->SetMat3("B_NormalInv", glm::inverse( normalMatrix ));

    sp->SetMat4("B_View",    m_viewMatrix);
    sp->SetMat4("B_ViewInv", m_viewMatrix.Inversed());

    sp->SetMat4("B_Projection",    m_projectionMatrix);
    sp->SetMat4("B_ProjectionInv", m_projectionMatrix.Inversed());

    Matrix4 pvmMatrix = m_projectionMatrix * m_viewMatrix * m_modelMatrix;
    sp->SetMat4("B_PVM", pvmMatrix);

    sp->UnBind();
}

void GLContext::Render(const VAO* vao, GL::RenderMode renderMode,
                       int elementsCount, int startIndex) const
{
    vao->Bind();
    glDrawArrays( static_cast<GLint>(renderMode), startIndex, elementsCount);
    vao->UnBind();
}

void GLContext::SetWriteDepth(bool writeDepth)
{
    if (writeDepth != m_writeDepth)
    {
        m_writeDepth = writeDepth;
        glDepthMask(m_writeDepth ? GL_TRUE : GL_FALSE);
    }
}

void GLContext::SetTestDepth(bool testDepth)
{
    if (m_testDepth != testDepth)
    {
        m_testDepth = testDepth;
        glDepthFunc(m_testDepth ? GL_LEQUAL : GL_ALWAYS);
    }
}

void GLContext::SetWireframe(bool wireframe)
{
    if (m_wireframe != wireframe)
    {
        m_wireframe = wireframe;
        glPolygonMode(GL_FRONT_AND_BACK, m_wireframe ? GL_LINE : GL_FILL);
    }
}

void GLContext::SetCullMode(GL::CullMode cullMode)
{
    if (cullMode != m_cullMode)
    {
        m_cullMode = cullMode;
        if (m_cullMode != GL::CullMode::None)
        {
            GL::Enable(GL_CULL_FACE);
            glCullFace(GLint(m_cullMode));
        }
        else { GL::Disable(GL_CULL_FACE); }
    }
}

void GLContext::SetModelMatrix(const Matrix4 &model)
{
    m_modelMatrix = model;
}

void GLContext::SetViewMatrix(const Matrix4 &view)
{
    m_viewMatrix = view;
}

void GLContext::SetProjectionMatrix(const Matrix4 &projection)
{
    m_projectionMatrix = projection;
}




bool GLContext::IsWriteDepth() const
{
    return m_writeDepth;
}

bool GLContext::IsTestDepth() const
{
    return m_testDepth;
}

bool GLContext::IsWireframe() const
{
    return m_wireframe;
}

bool GLContext::IsBound(GL::BindTarget bindTarget, GLId glId) const
{
    auto it = m_glBoundIds.Find(bindTarget);
    if (it != m_glBoundIds.End())
    {
        const std::stack<GLId>& boundIds = it->second;
        return !boundIds.empty() && boundIds.top() == glId;
    }
    return false;
}

GL::CullMode GLContext::GetCullMode() const
{
    return m_cullMode;
}

const Matrix4 &GLContext::GetModelMatrix() const
{
    return m_modelMatrix;
}

const Matrix4 &GLContext::GetViewMatrix() const
{
    return m_viewMatrix;
}

const Matrix4 &GLContext::GetProjectionMatrix() const
{
    return m_projectionMatrix;
}

GLId GLContext::GetBoundId(GL::BindTarget bindTarget) const
{
    if (!m_glBoundIds.ContainsKey(bindTarget)) { return 0; }
    const std::stack<GLId> &boundIds = m_glBoundIds.Get(bindTarget);
    return boundIds.empty() ? 0 : boundIds.top();
}

void GLContext::OnBind(GL::BindTarget bindTarget, GLId glId)
{
    if (!m_glBoundIds.ContainsKey(bindTarget))
    {
        m_glBoundIds.Set(bindTarget, std::stack<GLId>());
    }
    m_glBoundIds[bindTarget].push(glId);
}

void GLContext::OnUnBind(GL::BindTarget bindTarget)
{
    GLId previousBoundId = 0;
    if (m_glBoundIds.ContainsKey(bindTarget))
    {
        if (!m_glBoundIds[bindTarget].empty())
        {
            m_glBoundIds[bindTarget].pop();
        }
        previousBoundId = GetBoundId(bindTarget);
    }
    GL::_Bind(bindTarget, previousBoundId);
}

GLContext::GLContext()
{
}

GLContext::~GLContext()
{
}

