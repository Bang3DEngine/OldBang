#include "Bang/GLUniforms.h"

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

UniformBuffer<GLUniforms::Matrices> *GLUniforms::GetMatricesBuffer()
{
    return GLUniforms::GetBuffer<GLUniforms::Matrices>();
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
    _CreateBuffer<GLUniforms::Matrices>();
    _CreateBuffer<GLUniforms::Camera>();
    _CreateBuffer<GLUniforms::Viewport>();
}

void GLUniforms::BindAllUniformBuffersToShader(const ShaderProgram *sp)
{
    GL::BindUniformBufferToShader("MatricesBlock", sp, GetMatricesBuffer());
    GL::BindUniformBufferToShader("CameraBlock", sp, GetCameraBuffer());
    GL::BindUniformBufferToShader("ViewportBlock", sp, GetViewportBuffer());
}

void GLUniforms::SetModelMatrix(const Matrix4 &model)
{
    auto matrices = GLUniforms::GetMatricesBuffer();
    if (model != matrices->GetData()->model)
    {
        matrices->GetData()->model = model;
        Matrix3 normalMatrix = Matrix3(model.c0.xyz(),
                                       model.c1.xyz(),
                                       model.c2.xyz()
                                   ).Transposed().Inversed();
        matrices->GetData()->normal = Matrix4(normalMatrix);
        UpdatePVMMatrix();
    }

}
void GLUniforms::SetViewMatrix(const Matrix4 &view)
{
    auto matrices = GLUniforms::GetMatricesBuffer();
    if (view != matrices->GetData()->view)
    {
        matrices->GetData()->view    = view;
        matrices->GetData()->viewInv = view.Inversed();
        UpdatePVMMatrix();
    }
}
void GLUniforms::SetProjectionMatrix(const Matrix4 &projection)
{
    auto matrices = GLUniforms::GetMatricesBuffer();
    if (projection != matrices->GetData()->proj)
    {
        matrices->GetData()->proj    = projection;
        matrices->GetData()->projInv = projection.Inversed();
        UpdatePVMMatrix();
    }
}

void GLUniforms::UpdatePVMMatrix()
{
    auto matrices = GLUniforms::GetMatricesBuffer();
    GLUniforms *gu = GLUniforms::GetActive();

    Matrix4 pvmMatrix;
    const Matrix4 &model = matrices->GetData()->model;
    switch (gu->GetViewProjMode())
    {
        case GL::ViewProjMode::UseBoth:
            pvmMatrix = matrices->GetData()->proj *
                        matrices->GetData()->view *
                        model;
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

    matrices->GetData()->pvm = pvmMatrix;
    matrices->UpdateBuffer();
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

