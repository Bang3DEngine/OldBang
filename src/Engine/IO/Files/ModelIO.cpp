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

bool ModelIO::ReadModel(const Path &modelFilepath,
                        Array< RH<Mesh> > *meshes,
                        Array< RH<Material> > *materials)
{
    Assimp::Importer importer;
    const aiScene* scene = ReadScene(&importer, modelFilepath);

    bool ok = false;
    if (scene)
    {
        Array< RH<Material> > unorderedMaterials;
        for (int i = 0; i < SCAST<int>(scene->mNumMaterials); ++i)
        {
            RH<Material> material =
                    ModelIO::ReadMaterial(modelFilepath.GetDirectory(),
                                          scene->mMaterials[i]);
            unorderedMaterials.PushBack(material);
        }

        for (int i = 0; i < SCAST<int>(scene->mNumMeshes); ++i)
        {
            RH<Mesh> meshRH = ModelIO::ReadMesh(scene->mMeshes[i]);
            meshes->PushBack(meshRH);

            int matIndex = scene->mMeshes[i]->mMaterialIndex;
            RH<Material> mat = unorderedMaterials[matIndex];
            materials->PushBack(mat);
        }
        ok = true;
    }

    return ok;
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

RH<Material> ModelIO::ReadMaterial(const Path& modelDirectory,
                                   aiMaterial *aMaterial)
{
    RH<Material> materialRH = Resources::Create<Material>();

    aiString aMatName;
    aiGetMaterialString(aMaterial, AI_MATKEY_NAME, &aMatName);
    // String matName( aMatName.C_Str() );

    float aShininess;
    aiColor3D aDiffuseColor;
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

    materialRH.Get()->SetDiffuseColor( AIColor3ToColor(aDiffuseColor) );
    materialRH.Get()->SetShininess( aShininess );
    materialRH.Get()->SetTexture( matTexture.Get() );

    return materialRH;
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
            vertexNormals->PushBack(norm0);
            vertexNormals->PushBack(norm1);
            vertexNormals->PushBack(norm2);

            ASSERT( Math::Abs(norm0.Length()-1.0f) < 0.05f  );
            ASSERT( Math::Abs(norm1.Length()-1.0f) < 0.05f  );
            ASSERT( Math::Abs(norm2.Length()-1.0f) < 0.05f  );
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

RH<Mesh> ModelIO::ReadMesh(aiMesh *aMesh)
{
    RH<Mesh> meshRH = Resources::Create<Mesh>();

    Array<Vector3> vertexPositions;
    Array<Vector3> vertexNormals;
    Array<Vector2> vertexUvs;

    ModelIO::ReadMeshRaw(aMesh, &vertexPositions, &vertexNormals, &vertexUvs);

    meshRH.Get()->LoadAll(vertexPositions, vertexNormals, vertexUvs);

    return meshRH;
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
