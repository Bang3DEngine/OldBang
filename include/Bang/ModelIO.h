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
                          Array< RH<Material> > *materials,
                          Array<String> *meshesNames,
                          Array<String> *materialsNames);

    static bool ReadFirstFoundMeshRaw(const Path& modelFilepath,
                                      Array<Vector3> *vertexPositions,
                                      Array<Vector3> *vertexNormals,
                                      Array<Vector2> *vertexUvs);

    static void ReadMesh(aiMesh *aMesh,
                         RH<Mesh> *outMesh,
                         String *outMeshName);
    static void ReadMaterial(const Path& modelDirectory,
                             aiMaterial *aMaterial,
                             RH<Material> *outMaterial,
                             String *outMaterialName);

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
