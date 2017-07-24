#include "Bang/GLContext.h"

#include "GL/gl.h"
#include "GL/glew.h"

#include "Bang/G_VAO.h"
#include "Bang/Debug.h"
#include "Bang/Screen.h"
#include "Bang/Matrix4.h"
#include "Bang/G_ShaderProgram.h"

void GLContext::ApplyToShaderProgram(G_ShaderProgram *sp) const
{
    ENSURE(sp); ASSERT(GL::IsBound(sp));
    sp->SetMat4("B_Model",    m_modelMatrix);
    sp->SetMat4("B_ModelInv", m_modelMatrix.Inversed());

    Vector3 col0 = m_modelMatrix.c0.xyz();
    Vector3 col1 = m_modelMatrix.c1.xyz();
    Vector3 col2 = m_modelMatrix.c2.xyz();
    glm::mat3 normalMatrix = glm::mat3(glm::vec3(col0.x, col0.y, col0.z),
                                       glm::vec3(col1.x, col1.y, col1.z),
                                       glm::vec3(col2.x, col2.y, col2.z));
    normalMatrix = glm::inverse( glm::transpose(normalMatrix) );
    sp->SetMat3("B_Normal",    normalMatrix);
    sp->SetMat3("B_NormalInv", glm::inverse( normalMatrix ));

    sp->SetMat4("B_View",    m_viewMatrix);
    sp->SetMat4("B_ViewInv", m_viewMatrix.Inversed());

    sp->SetMat4("B_Projection",    m_projectionMatrix);
    sp->SetMat4("B_ProjectionInv", m_projectionMatrix.Inversed());

    sp->SetFloat("B_Camera_Near", m_zNear);
    sp->SetFloat("B_Camera_Far",  m_zFar);

    Matrix4 pvmMatrix;
    if (m_viewProjMode == GL::ViewProjMode::UseBoth)
    {
        pvmMatrix = m_projectionMatrix * m_viewMatrix * m_modelMatrix;
    }
    else if (m_viewProjMode == GL::ViewProjMode::OnlyFixAspectRatio)
    {
        Matrix4 modelTranslate( Vector4(1,0,0,0),
                                Vector4(0,1,0,0),
                                Vector4(0,0,1,0),
                                m_modelMatrix.c3);

        Matrix4 modelNoTranslate = m_modelMatrix;
        modelNoTranslate.SetTranslate( Vector3(0,0,0) );

        float ar = 1.0f / Screen::GetAspectRatio();
        Matrix4 fixAR(ar, 0, 0, 0,
                       0, 1, 0, 0,
                       0, 0, 1, 0,
                       0, 0, 0, 1);

        pvmMatrix = modelTranslate * fixAR * modelNoTranslate;
    }
    else if (m_viewProjMode == GL::ViewProjMode::IgnoreBoth)
    {
        pvmMatrix = m_modelMatrix;
    }

    sp->SetMat4("B_PVM", pvmMatrix);
}

void GLContext::Render(const G_VAO* vao, GL::RenderMode renderMode,
                       int elementsCount, int startIndex) const
{
    vao->Bind();
    glDrawArrays( static_cast<GLint>(renderMode), startIndex, elementsCount);
    vao->UnBind();
}

void GLContext::SetViewProjMode(GL::ViewProjMode mode)
{
    m_viewProjMode = mode;
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

void GLContext::SetZNearFar(float zNear, float zFar)
{
    m_zNear = zNear;
    m_zFar  = zFar;
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
    if (!m_glBoundIds.ContainsKey(bindTarget)) { return; }

    std::stack<GLId> &boundIds = m_glBoundIds.Get(bindTarget);
    if (!boundIds.empty())
    {
        const GLId currentId = GetBoundId(bindTarget);
        boundIds.pop();
        const GLId previousBoundId = GetBoundId(bindTarget);
        if (currentId != previousBoundId)
        {
            GL::_Bind(bindTarget, previousBoundId);
        }
    }
}

GLContext::GLContext()
{
}

GLContext::~GLContext()
{
}

