#include "Bang/ModelIO.h"

#include <fstream>
#include <sstream>
#include <cstdlib>
#include <stdio.h>
#include <cstring>
#include <iostream>

#include <assimp/scene.h>
#include <assimp/Exporter.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "Bang/Path.h"
#include "Bang/Mesh.h"
#include "Bang/Model.h"
#include "Bang/Paths.h"
#include "Bang/Scene.h"
#include "Bang/Debug.h"
#include "Bang/Array.h"
#include "Bang/Vector2.h"
#include "Bang/Vector3.h"
#include "Bang/Material.h"
#include "Bang/Resources.h"
#include "Bang/Texture2D.h"
#include "Bang/GameObject.h"
#include "Bang/MeshRenderer.h"
#include "Bang/XMLNodeReader.h"

USING_NAMESPACE_BANG

String AIStringToString(const aiString &str) { return String(str.C_Str()); }
Vector3 AIVectorToVec3(const aiVector3D &v) { return Vector3(v.x, v.y, v.z); }
Color AIColor3ToColor(const aiColor3D &c) { return Color(c.r, c.g, c.b, 1); }
Color AIColor4ToColor(const aiColor4D &c) { return Color(c.r, c.g, c.b, c.a); }
aiVector3D VectorToAIVec3(const Vector3 &v) { return aiVector3D(v.x, v.y, v.z); }
aiString StringToAIString(const String &str) { return aiString(str.ToCString()); }
aiColor3D ColorToAiColor3(const Color &c) { return aiColor3D(c.r, c.g, c.b); }
aiColor4D ColorToAiColor4(const Color &c) { return aiColor4D(c.r, c.g, c.b, c.a); }

int ModelIO::GetModelNumTriangles(const Path &modelFilepath)
{
    Assimp::Importer importer;
    const aiScene* scene = ImportScene(&importer, modelFilepath);

    if (scene && scene->HasMeshes()) { return scene->mMeshes[0]->mNumFaces; }
    return 0;
}

Tree<ModelIONode>* ReadModelNode(aiNode *node)
{
    Tree<ModelIONode>* modelNodeTree = new Tree<ModelIONode>();
    ModelIONode &modelNode = modelNodeTree->GetData();

    // Set name
    modelNode.name = AIStringToString(node->mName);

    // Set transformation
    const aiMatrix4x4 &m = node->mTransformation;
    modelNode.transformation =
           Matrix4(m[0][0], m[0][1], m[0][2], m[0][3],
                   m[1][0], m[1][1], m[1][2], m[1][3],
                   m[2][0], m[2][1], m[2][2], m[2][3],
                   m[3][0], m[3][1], m[3][2], m[3][3]);

    // Set mesh indices
    for (int i = 0; i < node->mNumMeshes; ++i)
    {
        uint meshIndex = node->mMeshes[i];
        modelNode.meshIndices.PushBack(meshIndex);
    }

    for (int i = 0; i < node->mNumChildren; ++i)
    {
        aiNode *child = node->mChildren[i];
        Tree<ModelIONode> *childModelTree = ReadModelNode(child);
        childModelTree->SetParent(modelNodeTree);
    }

    return modelNodeTree;
}

bool ModelIO::ImportModel(const Path& modelFilepath,
                          const GUID &modelGUID,
                          ModelIOScene *modelScene)
{
    Assimp::Importer importer;
    const aiScene* scene = ImportScene(&importer, modelFilepath);
    if (!scene) { return false; }

    int innerResourceGUID = 1;

    // Load materials
    Array< String > unorderedMaterialNames;
    Array< RH<Material> > unorderedMaterials;
    for (int i = 0; i < SCAST<int>(scene->mNumMaterials); ++i)
    {
        String materialName;
        RH<Material> materialRH;
        ModelIO::ImportMaterial(scene->mMaterials[i],
                              modelFilepath.GetDirectory(),
                              modelGUID,
                              innerResourceGUID,
                              &materialRH,
                              &materialName);

        unorderedMaterials.PushBack(materialRH);
        unorderedMaterialNames.PushBack(materialName);

        ++innerResourceGUID;
    }

    // Load meshes and store them into arrays
    for (int i = 0; i < SCAST<int>(scene->mNumMeshes); ++i)
    {
        RH<Mesh> meshRH;
        String meshName;
        ModelIO::ImportMesh(scene->mMeshes[i],
                          modelGUID,
                          innerResourceGUID,
                          &meshRH,
                          &meshName);

        int matIndex = scene->mMeshes[i]->mMaterialIndex;
        RH<Material> mat = unorderedMaterials[matIndex];
        const String &materialName = unorderedMaterialNames[matIndex];

        modelScene->meshes.PushBack(meshRH);
        modelScene->meshesNames.PushBack(meshName);

        modelScene->materials.PushBack(mat);
        modelScene->materialsNames.PushBack(materialName);

        ++innerResourceGUID;
    }

    modelScene->modelTree = ReadModelNode(scene->mRootNode);

    return true;
}

bool ModelIO::ImportFirstFoundMeshRaw(
                     const Path &modelFilepath,
                     Array<Mesh::VertexId> *vertexIndices,
                     Array<Vector3> *vertexPositionsPool,
                     Array<Vector3> *vertexNormalsPool,
                     Array<Vector2> *vertexUvsPool)
{
    Assimp::Importer importer;
    const aiScene* scene = ImportScene(&importer, modelFilepath);

    bool ok = false;
    if (scene && scene->HasMeshes())
    {
        ModelIO::ImportMeshRaw(scene->mMeshes[0],
                             vertexIndices,
                             vertexPositionsPool,
                             vertexNormalsPool,
                             vertexUvsPool);
        ok = true;
    }
    return ok;
}

void ModelIO::ImportMaterial(aiMaterial *aMaterial,
                             const Path& modelDirectory,
                             const GUID &parentModelGUID,
                             const GUID::GUIDType &innerMaterialGUID,
                             RH<Material> *outMaterial,
                             String *outMaterialName)
{
    *outMaterial =  Resources::CreateInnerResource<Material>(parentModelGUID,
                                                             innerMaterialGUID);

    aiString aMatName;
    aiGetMaterialString(aMaterial, AI_MATKEY_NAME, &aMatName);
    if (outMaterialName)
    {
        *outMaterialName = String( aMatName.C_Str() );
        if (outMaterialName->IsEmpty()) { *outMaterialName = "Material"; }
    }

    float aShininess;
    aiColor3D aAmbientColor;
    aiColor3D aDiffuseColor;
    aMaterial->Get(AI_MATKEY_COLOR_AMBIENT, aAmbientColor);
    aMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aDiffuseColor);
    aMaterial->Get(AI_MATKEY_SHININESS, aShininess);

    aiString aTexturePath;
    aMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &aTexturePath);
    Path texturePath ( String(aTexturePath.C_Str()) );
    texturePath = modelDirectory.Append(texturePath);

    RH<Texture2D> matTexture;
    if (texturePath.IsFile())
    {
        matTexture = Resources::Load<Texture2D>(texturePath);
    }

    outMaterial->Get()->SetDiffuseColor( AIColor3ToColor(aDiffuseColor) );
    outMaterial->Get()->SetShininess( aShininess );
    outMaterial->Get()->SetTexture( matTexture.Get() );
}

void ModelIO::ImportMeshRaw(
                  aiMesh *aMesh,
                  Array<Mesh::VertexId> *vertexIndices,
                  Array<Vector3> *vertexPositionsPool,
                  Array<Vector3> *vertexNormalsPool,
                  Array<Vector2> *vertexUvsPool)
{
    for (int i = 0; i < SCAST<int>(aMesh->mNumFaces); ++i)
    {
        for (int j = 0; j < aMesh->mFaces[i].mNumIndices; ++j)
        {
            Mesh::VertexId vIndex = aMesh->mFaces[i].mIndices[j];
            vertexIndices->PushBack(vIndex);
        }
    }

    // Positions
    for (int i = 0; i < SCAST<int>(aMesh->mNumVertices); ++i)
    {
        Vector3 pos = AIVectorToVec3(aMesh->mVertices[i]);
        vertexPositionsPool->PushBack(pos);
    }

    // Normals
    for (int i = 0; i < SCAST<int>(aMesh->mNumVertices); ++i)
    {
        Vector3 normal = AIVectorToVec3(aMesh->mNormals[i]);
        vertexNormalsPool->PushBack(normal);
    }

    // Uvs
    if (aMesh->GetNumUVChannels() > 0)
    {
        for (int i = 0; i < SCAST<int>(aMesh->mNumVertices); ++i)
        {
            Vector3 uvs = AIVectorToVec3(aMesh->mTextureCoords[0][i]);
            vertexUvsPool->PushBack( uvs.xy() );
        }
    }
}

void ModelIO::ExportModel(const GameObject *rootGameObject,
                          const Path &meshExportPath)
{
    Assimp::Exporter exporter;
    aiScene scene;

    // Create scene meshes, materials and textures
    Set<Mesh*> sceneMeshes;
    Set<Material*> sceneMaterials;
    Set<Texture2D*> sceneTextures;
    Map<Mesh*, Material*> sceneMeshToMaterial;
    Map<MeshRenderer*, Mesh*> meshRendererToMesh;
    Map<MeshRenderer*, Material*> meshRendererToMaterial;
    {
        List<MeshRenderer*> rootMRs =
                   rootGameObject->GetComponentsInChildren<MeshRenderer>(true);
        for (MeshRenderer *mr : rootMRs)
        {
            Mesh *mesh = mr->GetActiveMesh();
            if (mesh)
            {
                sceneMeshes.Add(mesh);
                meshRendererToMesh.Add(mr, mesh);
            }

            Material *material = mr->GetMaterial();
            if (material)
            {
                sceneMeshToMaterial.Add(mesh, material);

                sceneMaterials.Add(material);
                meshRendererToMaterial.Add(mr, material);

                if (material->GetTexture())
                {
                    sceneTextures.Add(material->GetTexture());
                }
            }
        }
    }

    if (sceneMeshes.IsEmpty()) { return; }

    Array<Mesh*> sceneMeshesArray(sceneMeshes.Begin(), sceneMeshes.End());
    Array<Texture2D*> sceneTexturesArray(sceneTextures.Begin(), sceneTextures.End());
    Array<Material*> sceneMaterialsArray(sceneMaterials.Begin(), sceneMaterials.End());

    // Create materials
    scene.mNumMaterials = sceneMaterialsArray.Size();
    scene.mMaterials = new aiMaterial*[scene.mNumMaterials];
    for (int i = 0; i < scene.mNumMaterials; ++i)
    {
        Material *material = sceneMaterialsArray[i];
        aiMaterial *aMaterial = MaterialToAiMaterial(material);;
        scene.mMaterials[i] = aMaterial;
    }

    // Create meshes
    scene.mNumMeshes = sceneMeshesArray.Size();
    scene.mMeshes = new aiMesh*[scene.mNumMeshes];
    for (int i = 0; i < scene.mNumMeshes; ++i)
    {
        Mesh *mesh = sceneMeshesArray[i];
        aiMesh *aMesh = MeshToAiMesh(mesh);
        if (sceneMeshToMaterial.ContainsKey(mesh))
        {
            Material *meshMaterial = sceneMeshToMaterial[mesh];
            aMesh->mMaterialIndex = sceneMaterialsArray.IndexOf(meshMaterial);
        }
        scene.mMeshes[i] = aMesh;
    }
    // scene.mNumTextures = sceneTexturesArray.Size();
    // scene.mTextures = new aiTexture*[scene.mNumTextures];

    scene.mRootNode = GameObjectToAiNode(rootGameObject, sceneMeshesArray);

    const String extension = meshExportPath.GetExtension();
    const String extensionId = GetExtensionIdFromExtension(extension).ToCString();
    const int result =
        exporter.Export(&scene,
                        extensionId,
                        meshExportPath.GetAbsolute().ToCString());

    if (result != AI_SUCCESS)
    {
        Debug_Error("Error exporting to " << meshExportPath << ": " <<
                    exporter.GetErrorString());
    }
}

String ModelIO::GetExtensionIdFromExtension(const String &extension)
{
    Assimp::Exporter exporter;
    for (int i = 0; i < exporter.GetExportFormatCount(); ++i)
    {
        const aiExportFormatDesc *fmtDesc = exporter.GetExportFormatDescription(i);
        if (extension.EqualsNoCase(fmtDesc->fileExtension))
        {
            return String(fmtDesc->id);
        }
    }
    Debug_Error("Can't export mesh to extension '" << extension << "'");
    return 0;
}

aiNode *ModelIO::GameObjectToAiNode(const GameObject *gameObject,
                                    const Array<Mesh*> &sceneMeshes)
{
    aiNode *goNode = new aiNode( gameObject->GetName() );
    goNode->mNumChildren = gameObject->GetChildren().Size();

    // Count number of meshes
    const List<MeshRenderer*> mrs = gameObject->GetComponents<MeshRenderer>();

    // Meshes
    goNode->mNumMeshes = 0; // Count number of meshes
    for (MeshRenderer *mr : mrs)
    {
        goNode->mNumMeshes += (mr->GetActiveMesh() ? 1 : 0);
    }
    goNode->mMeshes = new unsigned int[goNode->mNumMeshes];

    int i = 0; // Populate meshes indices
    for (MeshRenderer *mr : mrs)
    {
        if (mr->GetActiveMesh())
        {
            goNode->mMeshes[i] = sceneMeshes.IndexOf(mr->GetActiveMesh());
        }
    }

    // Add children if any
    if (goNode->mNumChildren > 0)
    {
        goNode->mChildren = new aiNode*[goNode->mNumChildren];
        for (int i = 0; i < goNode->mNumChildren; ++i)
        {
            GameObject *child = gameObject->GetChild(i);
            aiNode *childNode = GameObjectToAiNode(child, sceneMeshes);
            goNode->mChildren[i] = childNode;
            childNode->mParent = goNode;
        }
    }

    return goNode;
}

aiMesh *ModelIO::MeshToAiMesh(const Mesh *mesh)
{
    aiMesh *aMesh = new aiMesh();
    aMesh->mNumVertices = mesh->GetPositionsPool().Size();
    if (aMesh->mNumVertices > 0)
    {
        aMesh->mPrimitiveTypes     = 1;
        aMesh->mNumUVComponents[0] = 1;
        aMesh->mVertices         = new aiVector3D[aMesh->mNumVertices];
        aMesh->mNormals          = new aiVector3D[aMesh->mNumVertices];
        aMesh->mTextureCoords[0] = new aiVector3D[aMesh->mNumVertices];

         // Populate vertices
        for (int i = 0; i < aMesh->mNumVertices; ++i)
        {
            aMesh->mVertices[i] = VectorToAIVec3(mesh->GetPositionsPool()[i]);
            aMesh->mNormals[i]  = VectorToAIVec3(mesh->GetNormalsPool()[i]);
            aMesh->mTextureCoords[0][i] =
                            VectorToAIVec3(Vector3(mesh->GetUvsPool()[i], 0));
        }

        aMesh->mNumFaces = mesh->GetNumTriangles();
        aMesh->mFaces = new aiFace[aMesh->mNumFaces];
        for(int tri = 0; tri < aMesh->mNumFaces; ++tri)
        {
            aiFace *aFace = &(aMesh->mFaces[tri]);
            aFace->mNumIndices = 3;
            aFace->mIndices = new unsigned int[aFace->mNumIndices];
            const auto triIndices = mesh->GetTriangleVertexIndices(tri);
            for (int i = 0; i < 3; ++i) { aFace->mIndices[i] = triIndices[i]; }
        }
    }
    return aMesh;
}

aiMaterial *ModelIO::MaterialToAiMaterial(const Material *material)
{
    aiMaterial *aMaterial = new aiMaterial();
    // aMaterial->mNumProperties = 1;
    // aMaterial->mProperties = new aiMaterialProperty*[aMaterial->mNumProperties];

    aiColor3D diffColor = ColorToAiColor3(Color::Red);
    aMaterial->AddProperty(&diffColor, 1, AI_MATKEY_COLOR_DIFFUSE);/*
    aMaterial->mProperties[0] = new aiMaterialProperty();
    aMaterial->mProperties[0]->mKey = AI_MATKEY_COLOR_DIFFUSE;
    aMaterial->mProperties[0]->*/
    return aMaterial;
}

void ModelIO::ImportMesh(aiMesh *aMesh,
                         const GUID &parentModelGUID,
                         const GUID::GUIDType &innerMeshGUID,
                         RH<Mesh> *outMesh,
                         String *outMeshName)
{
    *outMesh =  Resources::CreateInnerResource<Mesh>(parentModelGUID, innerMeshGUID);

    Array<Mesh::VertexId> vertexIndices;
    Array<Vector3> vertexPositionsPool;
    Array<Vector3> vertexNormalsPool;
    Array<Vector2> vertexUvsPool;

    ModelIO::ImportMeshRaw(aMesh,
                         &vertexIndices,
                         &vertexPositionsPool,
                         &vertexNormalsPool,
                         &vertexUvsPool);
    if (outMeshName)
    {
        *outMeshName = String( aMesh->mName.C_Str() );
        if (outMeshName->IsEmpty()) { *outMeshName = "Mesh"; }
    }

    outMesh->Get()->LoadAll(vertexIndices,
                            vertexPositionsPool,
                            vertexNormalsPool,
                            vertexUvsPool);
}

const aiScene *ModelIO::ImportScene(Assimp::Importer *importer,
                                  const Path &modelFilepath)
{
    const aiScene* scene =
      importer->ReadFile(modelFilepath.GetAbsolute().ToCString(),
          aiProcess_Triangulate            |
          aiProcess_JoinIdenticalVertices  |
          aiProcess_GenSmoothNormals);

    return scene;
}
