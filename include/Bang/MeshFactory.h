#ifndef MESHFACTORY_H
#define MESHFACTORY_H

#include "Bang/ResourceHandle.h"

NAMESPACE_BANG_BEGIN

class MeshFactory
{
public:
    static RH<Mesh> GetPlane();
    static RH<Mesh> GetUIPlane();
    static RH<Mesh> GetCube();
    static RH<Mesh> GetSphere();
    static RH<Mesh> GetCone();
    static RH<Mesh> GetCamera();

private:
    Map<String, RH<Mesh>> m_cache;

    MeshFactory() = default;
    static RH<Mesh> GetMesh(const String &enginePath);

    static MeshFactory* GetActive();

    friend class Resources;
};

NAMESPACE_BANG_END

#endif // MESHFACTORY_H
