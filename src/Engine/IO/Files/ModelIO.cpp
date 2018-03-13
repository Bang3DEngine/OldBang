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

        unorderedMaterialNames.PushBack(materialName);
        unorderedMaterials.PushBack(materialRH);

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

bool ModelIO::ReadFirstFoundMeshRaw(const Path &modelFilepath,
                                    Array<Vector3> *vertexPositions,
                                    Array<Vector3> *vertexNormals,
                                    Array<Vector2> *vertexUvs)
{
    Assimp::Importer importer;
    const aiScene* scene = ReadScene(&importer, modelFilepath);

    bool ok = false;
    if (scene && scene->HasMeshes())
    {
        ModelIO::ReadMeshRaw(scene->mMeshes[0],
                             vertexPositions,
                             vertexNormals,
                             vertexUvs);
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

    outMaterial->Get()->SetDiffuseColor( Color::White );
    outMaterial->Get()->SetShininess( aShininess );
    outMaterial->Get()->SetTexture( matTexture.Get() );
}

void ModelIO::ReadMeshRaw(aiMesh *aMesh,
                          Array<Vector3> *vertexPositions,
                          Array<Vector3> *vertexNormals,
                          Array<Vector2> *vertexUvs)
{
    for (int i = 0; i < SCAST<int>(aMesh->mNumFaces); ++i)
    {
        if (aMesh->mFaces[i].mNumIndices < 3) { continue; }

        const int iV0 = aMesh->mFaces[i].mIndices[0];
        const int iV1 = aMesh->mFaces[i].mIndices[1];
        const int iV2 = aMesh->mFaces[i].mIndices[2];

        if (aMesh->HasPositions())
        {
            Vector3 v0 = AIVectorToVec3(aMesh->mVertices[iV0]);
            Vector3 v1 = AIVectorToVec3(aMesh->mVertices[iV1]);
            Vector3 v2 = AIVectorToVec3(aMesh->mVertices[iV2]);
            vertexPositions->PushBack(v0);
            vertexPositions->PushBack(v1);
            vertexPositions->PushBack(v2);
        }

        if (aMesh->HasNormals())
        {
            Vector3 norm0 = AIVectorToVec3(aMesh->mNormals[iV0]);
            Vector3 norm1 = AIVectorToVec3(aMesh->mNormals[iV1]);
            Vector3 norm2 = AIVectorToVec3(aMesh->mNormals[iV2]);
            vertexNormals->PushBack(norm0.NormalizedSafe());
            vertexNormals->PushBack(norm1.NormalizedSafe());
            vertexNormals->PushBack(norm2.NormalizedSafe());
        }

        if (aMesh->GetNumUVChannels() > 0)
        {
            Vector2 uv0 = AIVectorToVec3(aMesh->mTextureCoords[0][iV0]).xy();
            Vector2 uv1 = AIVectorToVec3(aMesh->mTextureCoords[0][iV1]).xy();
            Vector2 uv2 = AIVectorToVec3(aMesh->mTextureCoords[0][iV2]).xy();
            vertexUvs->PushBack(uv0);
            vertexUvs->PushBack(uv1);
            vertexUvs->PushBack(uv2);
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

    Array<Vector3> vertexPositions;
    Array<Vector3> vertexNormals;
    Array<Vector2> vertexUvs;

    ModelIO::ReadMeshRaw(aMesh, &vertexPositions, &vertexNormals, &vertexUvs);
    if (outMeshName)
    {
        *outMeshName = String( aMesh->mName.C_Str() );
        if (outMeshName->IsEmpty()) { *outMeshName = "Mesh"; }
    }

    outMesh->Get()->LoadAll(vertexPositions, vertexNormals, vertexUvs);
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
