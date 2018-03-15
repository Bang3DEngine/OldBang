#ifndef MESHFACTORY_H
#define MESHFACTORY_H

#include "Bang/Map.h"
#include "Bang/Path.h"
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

protected:
    MeshFactory() = default;
    virtual ~MeshFactory() = default;

    static RH<Mesh> GetMesh(const String &enginePath);
    static RH<Mesh> GetMesh(const Path &fullPath);

private:
    Map<Path, RH<Mesh>> m_cache;

    static MeshFactory* GetActive();

    friend class Resources;
};

NAMESPACE_BANG_END

#endif // MESHFACTORY_H
