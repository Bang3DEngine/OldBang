#include "Bang/GLUniforms.h"

#include "Bang/UniformBuffer.h"

USING_NAMESPACE_BANG

IUniformBuffer::IUniformBuffer()
{
}

IUniformBuffer::~IUniformBuffer()
{
}

void IUniformBuffer::SetBindingPoint(int bindingPoint)
{
    m_bindingPoint = bindingPoint;
    Bind();
    glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, GetGLId());
    UnBind();
}

GLuint IUniformBuffer::GetBindingPoint() const { return m_bindingPoint; }



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
    matrices->GetData()->model = model;
    Matrix3 normalMatrix = Matrix3(model.c0.xyz(),
                                   model.c1.xyz(),
                                   model.c2.xyz()
                               ).Transposed().Inversed();
    matrices->GetData()->normal = Matrix4(normalMatrix);
    UpdatePVMMatrix();

}
void GLUniforms::SetViewMatrix(const Matrix4 &view)
{
    auto matrices = GLUniforms::GetMatricesBuffer();
    matrices->GetData()->view    = view;
    matrices->GetData()->viewInv = view.Inversed();
    UpdatePVMMatrix();
}
void GLUniforms::SetProjectionMatrix(const Matrix4 &projection)
{
    auto matrices = GLUniforms::GetMatricesBuffer();
    matrices->GetData()->proj    = projection;
    matrices->GetData()->projInv = projection.Inversed();
    UpdatePVMMatrix();
}

void GLUniforms::UpdatePVMMatrix()
{
    auto matrices = GLUniforms::GetMatricesBuffer();

    Matrix4 pvmMatrix;
    Matrix4 model = matrices->GetData()->model;
    if (GL::GetViewProjMode() == GL::ViewProjMode::UseBoth)
    {
        pvmMatrix = matrices->GetData()->proj *
                    matrices->GetData()->view *
                    model;
    }
    else if (GL::GetViewProjMode() == GL::ViewProjMode::OnlyFixAspectRatio)
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
    else if (GL::GetViewProjMode() == GL::ViewProjMode::IgnoreBoth)
    {
        pvmMatrix = model;
    }
    else if (GL::GetViewProjMode() == GL::ViewProjMode::IgnoreBothAndModel)
    {
        pvmMatrix = Matrix4::Identity;
    }

    matrices->GetData()->pvm = pvmMatrix;
    matrices->UpdateBuffer();
}

GLUniforms::~GLUniforms()
{
    while (!m_uniformBuffers.IsEmpty())
    {
        RemoveBuffer(m_uniformBuffers.Begin()->second);
    }
}

GLUniforms *GLUniforms::GetActive()
{
    return GL::GetActive()->GetGLUniforms();
}

