#include "Bang/Batch.h"

#include "Bang/Mesh.h"
#include "Bang/Material.h"
#include "Bang/Resources.h"
#include "Bang/GLUniforms.h"

USING_NAMESPACE_BANG

Bang::Batch::Batch(const Bang::BatchParameters &params)
{
    m_params = params;
    m_mesh = Resources::Create<Mesh>();
}

Batch::~Batch()
{
}

void Batch::AddGeometry(const Array<Vector3> &positions,
                        const Array<Vector3> &normals,
                        const Array<Vector2> &uvs)
{
    ASSERT(positions.Size() == uvs.Size() && uvs.Size() == normals.Size());

    m_positions.PushBack(positions);
    m_normals.PushBack(normals);
    m_uvs.PushBack(uvs);
}

void Batch::Render() const
{
    GetParameters().GetMaterial()->Bind();

    GLUniforms::SetModelMatrix(GetParameters().GetTransform());
    GLUniforms::SetViewMatrix(Matrix4::Identity);
    GLUniforms::SetProjectionMatrix(Matrix4::Identity);
    GL::SetViewProjMode(GL::ViewProjMode::IgnoreBoth);

    m_mesh.Get()->LoadAll(m_positions, m_normals, m_uvs);

    GL::DrawArrays(m_mesh.Get()->GetVAO(),
                   GetParameters().GetPrimitive(),
                   m_mesh.Get()->GetPositions().Size(),
                   0);

    // GetParameters().GetMaterial()->UnBind();
}

const BatchParameters &Batch::GetParameters() const
{
    return m_params;
}
