#include "Bang/BatchParameters.h"

#include "Bang/Material.h"
#include "Bang/Texture2D.h"

USING_NAMESPACE_BANG

BatchParameters::BatchParameters()
{
}

BatchParameters::~BatchParameters()
{
}

void BatchParameters::SetMaterial(Material *material)
{
    p_material = material;
}

void BatchParameters::SetTransform(const Matrix4 &transform)
{
    m_transform = transform;
}

void BatchParameters::SetPrimitive(GL::Primitive primitive)
{
    m_primitive = primitive;
}

bool BatchParameters::Equals(const BatchParameters &bp) const
{
    if (GetPrimitive() != bp.GetPrimitive()) { return false; }

    bool matEquals = false;
    {
        Material *mat = GetMaterial();
        Material *bpMat = bp.GetMaterial();

        if (mat == bpMat) { matEquals = true; }
        if (!matEquals && mat && bpMat)
        {
            matEquals =
              (mat->GetTexture() == bpMat->GetTexture()) &&
              (mat->GetShaderProgram() == bpMat->GetShaderProgram()) &&
              (mat->GetDiffuseColor() == bpMat->GetDiffuseColor());
        }
    }
    if (!matEquals) { return false; }

    if (GetTransform() != bp.GetTransform()) { return false; }

    return true;
}

const Matrix4 &BatchParameters::GetTransform() const
{
    return m_transform;
}

Material *BatchParameters::GetMaterial() const
{
    return p_material;
}

GL::Primitive BatchParameters::GetPrimitive() const
{
    return m_primitive;
}
