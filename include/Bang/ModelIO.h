#ifndef MODELIO_H
#define MODELIO_H

#include "Bang/Mesh.h"
#include "Bang/Tree.h"
#include "Bang/Array.h"
#include "Bang/Matrix4.h"
#include "Bang/ResourceHandle.h"

FORWARD class aiMesh;
FORWARD class aiNode;
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

    static bool ImportModel(const Path& modelFilepath,
                            const GUID &modelGUID,
                            ModelIOScene *modelScene);

    static bool ImportFirstFoundMeshRaw(
                     const Path& modelFilepath,
                     Array<Mesh::VertexId> *vertexIndices,
                     Array<Vector3> *vertexPositions,
                     Array<Vector3> *vertexNormals,
                     Array<Vector2> *vertexUvs);

    static void ImportMesh(aiMesh *aMesh,
                           const GUID &parentModelGUID,
                           const GUID::GUIDType &innerMeshGUID,
                           RH<Mesh> *outMesh,
                           String *outMeshName);
    static void ImportMaterial(aiMaterial *aMaterial,
                               const Path& modelDirectory,
                               const GUID &parentModelGUID,
                               const GUID::GUIDType &innerMaterialGUID,
                               RH<Material> *outMaterial,
                               String *outMaterialName);

    static void ImportMeshRaw(
                     aiMesh *aMesh,
                     Array<Mesh::VertexId> *vertexIndices,
                     Array<Vector3> *vertexPositionsPool,
                     Array<Vector3> *vertexNormalsPool,
                     Array<Vector2> *vertexUvsPool);

    static void ExportModel(const GameObject *gameObject,
                            const Path &meshExportPath);

    ModelIO() = delete;

private:
    static String GetExtensionIdFromExtension(const String &extension);
    static aiNode* GameObjectToAiNode(const GameObject *gameObject,
                                      const Array<Mesh*> &sceneMeshes);
    static aiMesh* MeshToAiMesh(const Mesh *mesh);
    static aiMaterial* MaterialToAiMaterial(const Material *material);
    static const aiScene *ImportScene(Assimp::Importer *importer,
                                      const Path& modelFilepath);
};

NAMESPACE_BANG_END

#endif // MODELIO_H
