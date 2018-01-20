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

GLUniforms::Matrices *GLUniforms::GetMatrices()
{
    return &GLUniforms::GetActive()->m_matrices;
}

UniformBuffer<GLUniforms::Camera> *GLUniforms::GetCameraBuffer()
{
    return GLUniforms::GetBuffer<GLUniforms::Camera>();
}

UniformBuffer<GLUniforms::Viewport> *GLUniforms::GetViewportBuffer()
{
    return GLUniforms::GetBuffer<GLUniforms::Viewport>();
}

GLUniforms::GLUniforms()
{
    _CreateBuffer<GLUniforms::Camera>();
    _CreateBuffer<GLUniforms::Viewport>();
}

void GLUniforms::SetAllUniformsToShaderProgram(ShaderProgram *sp)
{
    if (GL::IsBound(sp->GetGLBindTarget(), sp->GetGLId()))
    {
        Matrices *matrices = GLUniforms::GetMatrices();
        sp->Set("B_Model", matrices->model, false);
        sp->Set("B_Normal", matrices->normal, false);
        sp->Set("B_View", matrices->view, false);
        sp->Set("B_Projection", matrices->proj, false);
        sp->Set("B_PVM", matrices->pvm, false);
    }
}

void GLUniforms::BindAllUniformBuffersToShader(const ShaderProgram *sp)
{
    GL::BindUniformBufferToShader("CameraBlock", sp, GetCameraBuffer());
    GL::BindUniformBufferToShader("ViewportBlock", sp, GetViewportBuffer());
}

void GLUniforms::SetModelMatrix(const Matrix4 &model)
{
    Matrices *matrices = GLUniforms::GetMatrices();
    if (model != matrices->model)
    {
        matrices->model = model;
        Matrix3 normalMatrix = Matrix3(model.c0.xyz(),
                                       model.c1.xyz(),
                                       model.c2.xyz()
                                   ).Transposed().Inversed();
        matrices->normal = normalMatrix;
        UpdatePVMMatrix();
    }

}
void GLUniforms::SetViewMatrix(const Matrix4 &view)
{
    Matrices *matrices = GLUniforms::GetMatrices();
    if (view != matrices->view)
    {
        matrices->view    = view;
        matrices->viewInv = view.Inversed();
        UpdatePVMMatrix();
    }
}
void GLUniforms::SetProjectionMatrix(const Matrix4 &projection)
{
    Matrices *matrices = GLUniforms::GetMatrices();
    if (projection != matrices->proj)
    {
        matrices->proj    = projection;
        matrices->projInv = projection.Inversed();
        UpdatePVMMatrix();
    }
}

void GLUniforms::UpdatePVMMatrix()
{
    Matrices *matrices = GLUniforms::GetMatrices();
    GLUniforms *gu = GLUniforms::GetActive();

    Matrix4 pvmMatrix;
    const Matrix4 &model = matrices->model;
    switch (gu->GetViewProjMode())
    {
        case GL::ViewProjMode::UseBoth:
            pvmMatrix = matrices->proj * matrices->view * model;
        break;

        case GL::ViewProjMode::OnlyFixAspectRatio:
        {
            Matrix4 modelTranslate( Vector4(1,0,0,0),
                                    Vector4(0,1,0,0),
                                    Vector4(0,0,1,0),
                                    model.c3);

            Matrix4 modelNoTranslate = model;
            modelNoTranslate.SetTranslate( Vector3(0,0,0) );

            float ar = 1.0f / GL::GetViewportAspectRatio();
            Matrix4 fixAR(ar, 0, 0, 0,
                           0, 1, 0, 0,
                           0, 0, 1, 0,
                           0, 0, 0, 1);

            pvmMatrix = modelTranslate * fixAR * modelNoTranslate;
        }
        break;

        case GL::ViewProjMode::IgnoreBoth:
            pvmMatrix = model;
            break;

        case GL::ViewProjMode::IgnoreBothAndModel:
            pvmMatrix = Matrix4::Identity;
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

