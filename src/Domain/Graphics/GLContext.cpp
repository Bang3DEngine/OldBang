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
#include "ShaderContract.h"

void GLContext::ApplyToShaderProgram(ShaderProgram *sp) const
{
    sp->Bind();

    Camera *cam = SceneManager::GetActiveScene()->GetCamera();
    sp->SetVec3(ShaderContract::Uniform_Position_Camera,
                cam->gameObject->transform->GetPosition());

    sp->SetMat4(ShaderContract::Uniform_Matrix_Model, m_modelMatrix);
    sp->SetMat4(ShaderContract::Uniform_Matrix_Model_Inverse, m_modelMatrix.Inversed());

    Matrix4 normalMatrix = m_modelMatrix.Inversed().Transposed();
    sp->SetMat4(ShaderContract::Uniform_Matrix_Normal, normalMatrix);
    sp->SetMat4(ShaderContract::Uniform_Matrix_Normal_Inverse, normalMatrix.Inversed());

    sp->SetMat4(ShaderContract::Uniform_Matrix_View, m_viewMatrix);
    sp->SetMat4(ShaderContract::Uniform_Matrix_View_Inverse, m_viewMatrix.Inversed());

    sp->SetMat4(ShaderContract::Uniform_Matrix_Projection, m_projectionMatrix);
    sp->SetMat4(ShaderContract::Uniform_Matrix_Projection_Inverse, m_projectionMatrix.Inversed());

    Matrix4 pvmMatrix = m_projectionMatrix * m_viewMatrix * m_modelMatrix;
    sp->SetMat4(ShaderContract::Uniform_Matrix_PVM, pvmMatrix);

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
    m_writeDepth = writeDepth;
    glDepthMask(m_writeDepth ? GL_TRUE : GL_FALSE);
}

void GLContext::SetTestDepth(bool testDepth)
{
    m_testDepth = testDepth;
    glDepthFunc(m_testDepth ? GL_LEQUAL : GL_ALWAYS);
}

void GLContext::SetWireframe(bool wireframe)
{
    m_wireframe = wireframe;
    glPolygonMode(GL_FRONT_AND_BACK, m_wireframe ? GL_LINE : GL_FILL);
}

void GLContext::SetCullMode(GL::CullMode cullMode)
{
    m_cullMode = cullMode;
    if (m_cullMode != GL::CullMode::None)
    {
        glEnable(GL_CULL_FACE);
        glCullFace(GLint(m_cullMode));
    }
    else { glDisable(GL_CULL_FACE); }
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



GLContext::GLContext()
{
}

GLContext::~GLContext()
{
}

