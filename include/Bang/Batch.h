#ifndef BATCH_H
#define BATCH_H

#include "Bang/ResourceHandle.h"
#include "Bang/BatchParameters.h"

NAMESPACE_BANG_BEGIN

FORWARD class Mesh;

class Batch
{
public:
    Batch(const BatchParameters &params);
	virtual ~Batch();

    void AddGeometry(const Array<Vector3> &positions,
                     const Array<Vector3> &normals,
                     const Array<Vector2> &uvs);
    void Render() const;

    const BatchParameters& GetParameters() const;

private:
    Array<Vector3> m_positions;
    Array<Vector3> m_normals;
    Array<Vector2> m_uvs;

    RH<Mesh> m_mesh;
    BatchParameters m_params;
};

NAMESPACE_BANG_END

#endif // BATCH_H

