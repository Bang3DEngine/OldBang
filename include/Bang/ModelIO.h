#ifndef MODELIO_H
#define MODELIO_H

#include "Bang/Array.h"
#include "Bang/ResourceHandle.h"

FORWARD class aiMesh;
FORWARD class aiScene;
FORWARD class aiMaterial;
FORWARD namespace Assimp { FORWARD class Importer; }

NAMESPACE_BANG_BEGIN

FORWARD class Scene;
FORWARD class Texture2D;

class ModelIO
{
public:
    static int GetModelNumTriangles(const Path& modelFilepath);

    static bool ReadModel(const Path& modelFilepath,
                          Array< RH<Mesh> > *meshes,
                          Array< RH<Material> > *materials);

    static bool ReadFirstFoundMeshRaw(const Path& modelFilepath,
                                      Array<Vector3> *vertexPositions,
                                      Array<Vector3> *vertexNormals,
                                      Array<Vector2> *vertexUvs);

    static RH<Mesh> ReadMesh(aiMesh *aMesh);
    static RH<Material> ReadMaterial(const Path& modelDirectory,
                                     aiMaterial *aMaterial);

    static void ReadMeshRaw(aiMesh *aMesh,
                            Array<Vector3> *vertexPositions,
                            Array<Vector3> *vertexNormals,
                            Array<Vector2> *vertexUvs);

    ModelIO() = delete;

private:    
    static const aiScene *ReadScene(Assimp::Importer *importer,
                                    const Path& modelFilepath);
};

NAMESPACE_BANG_END

#endif // MODELIO_H
