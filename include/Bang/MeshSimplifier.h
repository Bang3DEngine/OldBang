#ifndef MESHSIMPLIFIER_H
#define MESHSIMPLIFIER_H

#include "Bang/Bang.h"
#include "Bang/Mesh.h"
#include "Bang/ResourceHandle.h"

NAMESPACE_BANG_BEGIN

class MeshSimplifier
{
public:
    static Array<RH<Mesh>> GetAllMeshLODs(const Mesh *mesh);

    MeshSimplifier() = delete;
};

NAMESPACE_BANG_END

#endif // MESHSIMPLIFIER_H

