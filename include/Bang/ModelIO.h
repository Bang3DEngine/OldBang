#ifndef MODELIO_H
#define MODELIO_H

#include "Bang/Mesh.h"
#include "Bang/Tree.h"
#include "Bang/Array.h"
#include "Bang/Matrix4.h"
#include "Bang/ResourceHandle.h"

FORWARD class aiMesh;
FORWARD class aiScene;
FORWARD class aiMaterial;
FORWARD namespace Assimp
{
    FORWARD class Importer;
}

NAMESPACE_BANG_BEGIN

FORWARD class Scene;
FORWARD class Model;
FORWARD class Texture2D;

struct ModelIONode
{
    String name;
    Matrix4 transformation;

    Array<uint> meshIndices;
    Array<uint> meshMaterialIndices;
};

struct ModelIOScene
{
    Array< RH<Mesh> > meshes;
    Array< String > meshesNames;

    Array< RH<Material> > materials;
    Array< String > materialsNames;

    Tree<ModelIONode> *modelTree = nullptr;

    ~ModelIOScene() { if (modelTree) { delete modelTree; } }
};

class ModelIO
{
public:
    static int GetModelNumTriangles(const Path& modelFilepath);

    static bool ReadModel(const Path& modelFilepath,
                          const GUID &modelGUID,
                          ModelIOScene *modelScene);

    static bool ReadFirstFoundMeshRaw(
                     const Path& modelFilepath,
                     Array<Mesh::VertexId> *vertexIndices,
                     Array<Vector3> *vertexPositions,
                     Array<Vector3> *vertexNormals,
                     Array<Vector2> *vertexUvs);

    static void ReadMesh(aiMesh *aMesh,
                         const GUID &parentModelGUID,
                         const GUID::GUIDType &innerMeshGUID,
                         RH<Mesh> *outMesh,
                         String *outMeshName);
    static void ReadMaterial(aiMaterial *aMaterial,
                             const Path& modelDirectory,
                             const GUID &parentModelGUID,
                             const GUID::GUIDType &innerMaterialGUID,
                             RH<Material> *outMaterial,
                             String *outMaterialName);

    static void ReadMeshRaw(
                     aiMesh *aMesh,
                     Array<Mesh::VertexId> *vertexIndices,
                     Array<Vector3> *vertexPositionsPool,
                     Array<Vector3> *vertexNormalsPool,
                     Array<Vector2> *vertexUvsPool);

    ModelIO() = delete;

private:
    static const aiScene *ReadScene(Assimp::Importer *importer,
                                    const Path& modelFilepath);
};

NAMESPACE_BANG_END

#endif // MODELIO_H
