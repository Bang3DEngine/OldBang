#include "Bang/ModelIO.h"

#include <fstream>
#include <sstream>
#include <cstdlib>
#include <stdio.h>
#include <cstring>
#include <iostream>

#include <assimp/scene.h>
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
#include "Bang/XMLNodeReader.h"

USING_NAMESPACE_BANG

String AIStringToString(const aiString &str) { return String(str.C_Str()); }
Vector3 AIVectorToVec3(const aiVector3D &v) { return Vector3(v.x, v.y, v.z); }
Color AIColor3ToColor(const aiColor3D &c) { return Color(c.r, c.g, c.b, 1); }
Color AIColor4ToColor(const aiColor4D &c) { return Color(c.r, c.g, c.b, c.a); }

int ModelIO::GetModelNumTriangles(const Path &modelFilepath)
{
    Assimp::Importer importer;
    const aiScene* scene = ReadScene(&importer, modelFilepath);

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

bool ModelIO::ReadModel(const Path& modelFilepath,
                        const GUID &modelGUID,
                        ModelIOScene *modelScene)
{
    Assimp::Importer importer;
    const aiScene* scene = ReadScene(&importer, modelFilepath);
    if (!scene) { return false; }

    int innerResourceGUID = 1;

    // Load materials
    Array< String > unorderedMaterialNames;
    Array< RH<Material> > unorderedMaterials;
    for (int i = 0; i < SCAST<int>(scene->mNumMaterials); ++i)
    {
        String materialName;
        RH<Material> materialRH;
        ModelIO::ReadMaterial(scene->mMaterials[i],
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
        ModelIO::ReadMesh(scene->mMeshes[i],
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

bool ModelIO::ReadFirstFoundMeshRaw(
                     const Path &modelFilepath,
                     Array<Mesh::VertexId> *vertexIndices,
                     Array<Vector3> *vertexPositionsPool,
                     Array<Vector3> *vertexNormalsPool,
                     Array<Vector2> *vertexUvsPool)
{
    Assimp::Importer importer;
    const aiScene* scene = ReadScene(&importer, modelFilepath);

    bool ok = false;
    if (scene && scene->HasMeshes())
    {
        ModelIO::ReadMeshRaw(scene->mMeshes[0],
                             vertexIndices,
                             vertexPositionsPool,
                             vertexNormalsPool,
                             vertexUvsPool);
        ok = true;
    }
    return ok;
}

void ModelIO::ReadMaterial(aiMaterial *aMaterial,
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

void ModelIO::ReadMeshRaw(
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

void ModelIO::ReadMesh(aiMesh *aMesh,
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

    ModelIO::ReadMeshRaw(aMesh,
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

const aiScene *ModelIO::ReadScene(Assimp::Importer *importer,
                                  const Path &modelFilepath)
{
    const aiScene* scene =
      importer->ReadFile(modelFilepath.GetAbsolute().ToCString(),
          aiProcess_Triangulate            |
          aiProcess_JoinIdenticalVertices  |
          aiProcess_GenSmoothNormals);

    return scene;
}
