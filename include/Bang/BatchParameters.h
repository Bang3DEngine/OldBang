#ifndef BATCHPARAMETERS_H
#define BATCHPARAMETERS_H

#include "Bang/GL.h"
#include "Bang/Matrix4.h"

NAMESPACE_BANG_BEGIN

FORWARD class Material;

class BatchParameters
{
public:
	BatchParameters();
	virtual ~BatchParameters();

    void SetMaterial(Material *material);
    void SetTransform(const Matrix4 &transform);
    void SetPrimitive(GL::Primitive primitive);

    bool Equals(const BatchParameters &batchParams) const;
    const Matrix4& GetTransform() const;
    Material* GetMaterial() const;
    GL::Primitive GetPrimitive() const;

private:
    Matrix4 m_transform = Matrix4::Identity;
    GL::Primitive m_primitive = GL::Primitive::Triangles;
    Material *p_material = nullptr;
};

NAMESPACE_BANG_END

#endif // BATCHPARAMETERS_H

