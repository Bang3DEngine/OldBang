#include "Bang/GLUniforms.h"

#include "Bang/ShaderProgram.h"
#include "Bang/UniformBuffer.h"

USING_NAMESPACE_BANG

GLUniforms::~GLUniforms()
{
    while (!m_uniformBuffers.IsEmpty())
    {
        RemoveBuffer(m_uniformBuffers.Begin()->second);
    }
}

void GLUniforms::RemoveBuffer(IUniformBuffer *buffer)
{
    if (m_uniformBuffers.ContainsValue(buffer))
    {
        delete buffer;
        m_uniformBuffers.RemoveValues(buffer);
    }
}

GLUniforms::MatrixUniforms *GLUniforms::GetMatrixUniforms()
{
    return &GLUniforms::GetActive()->m_matrixUniforms;
}

GLUniforms::CameraUniforms *GLUniforms::GetCameraUniforms()
{
    return &GLUniforms::GetActive()->m_cameraUniforms;
}

UniformBuffer<GLUniforms::ViewportUniforms> *GLUniforms::GetViewportBuffer()
{
    return GLUniforms::GetBuffer<GLUniforms::ViewportUniforms>();
}

GLUniforms::GLUniforms()
{
    _CreateBuffer<GLUniforms::ViewportUniforms>();
}

void GLUniforms::SetAllUniformsToShaderProgram(ShaderProgram *sp)
{
    if (GL::IsBound(sp->GetGLBindTarget(), sp->GetGLId()))
    {
        MatrixUniforms *matrices = GLUniforms::GetMatrixUniforms();
        sp->Set("B_Model", matrices->model, false);
        sp->Set("B_Normal", matrices->normal, false);
        sp->Set("B_View", matrices->view, false);
        sp->Set("B_Projection", matrices->proj, false);
        sp->Set("B_PVM", matrices->pvm, false);

        CameraUniforms *cameraUniforms = GLUniforms::GetCameraUniforms();
        sp->Set("B_Camera_ZNear", cameraUniforms->zNear, false);
        sp->Set("B_Camera_ZFar", cameraUniforms->zFar, false);
    }
}

void GLUniforms::BindAllUniformBuffersToShader(const ShaderProgram *sp)
{
    GL::BindUniformBufferToShader("ViewportBlock", sp, GetViewportBuffer());
}

void GLUniforms::SetCameraUniforms(float zNear, float zFar)
{
    CameraUniforms *cameraUniforms = GLUniforms::GetCameraUniforms();
    cameraUniforms->zNear = zNear;
    cameraUniforms->zFar  = zFar;
}

void GLUniforms::SetModelMatrix(const Matrix4 &model)
{
    MatrixUniforms *matrices = GLUniforms::GetMatrixUniforms();
    if (model != matrices->model)
    {
        matrices->model = model;
        UpdatePVMMatrix();
    }

}
void GLUniforms::SetViewMatrix(const Matrix4 &view)
{
    MatrixUniforms *matrices = GLUniforms::GetMatrixUniforms();
    if (view != matrices->view)
    {
        matrices->view    = view;
        matrices->viewInv = view.Inversed();
        UpdatePVMMatrix();
    }
}
void GLUniforms::SetProjectionMatrix(const Matrix4 &projection)
{
    MatrixUniforms *matrices = GLUniforms::GetMatrixUniforms();
    if (projection != matrices->proj)
    {
        matrices->proj    = projection;
        matrices->projInv = projection.Inversed();
        UpdatePVMMatrix();
    }
}

void GLUniforms::UpdatePVMMatrix()
{
    MatrixUniforms *matrices = GLUniforms::GetMatrixUniforms();
    GLUniforms *gu = GLUniforms::GetActive();

    Matrix4 pvmMatrix;
    const Matrix4 &model = matrices->model;
    Matrix4 viewModel = matrices->view * matrices->model;

    Matrix4 normalMatrix = Matrix4(viewModel.c0,
                                   viewModel.c1,
                                   viewModel.c2,
                                   viewModel.c3).Inversed().Transposed();
    matrices->normal = normalMatrix;

    switch (gu->GetViewProjMode())
    {
        case GL::ViewProjMode::World:
            pvmMatrix = matrices->proj * viewModel;
        break;

        case GL::ViewProjMode::Canvas:
            pvmMatrix = model;
        break;
    }

    matrices->pvm = pvmMatrix;
}

void GLUniforms::SetViewProjMode(GL::ViewProjMode viewProjMode)
{
    if (viewProjMode != GetViewProjMode())
    {
        m_viewProjMode = viewProjMode;
        UpdatePVMMatrix();
    }
}

GL::ViewProjMode GLUniforms::GetViewProjMode() const
{
    return m_viewProjMode;
}

GLUniforms *GLUniforms::GetActive()
{
    return GL::GetActive()->GetGLUniforms();
}

