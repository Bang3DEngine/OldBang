#include "Bang/FileReader.h"

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
#include "Bang/Paths.h"
#include "Bang/Scene.h"
#include "Bang/Debug.h"
#include "Bang/Array.h"
#include "Bang/Vector2.h"
#include "Bang/Vector3.h"
#include "Bang/XMLParser.h"
#include "Bang/GameObject.h"

int FileReader::GetModelNumTriangles(const Path &filepath)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filepath.GetAbsolute().ToCString(),
          aiProcess_Triangulate            |
          aiProcess_JoinIdenticalVertices);
    if (scene && scene->HasMeshes()) { return scene->mMeshes[0]->mNumFaces; }
    return 0;
}

Vector3 AIVectorToVec3(const aiVector3D &v) { return Vector3(v.x, v.y, v.z); }

bool FileReader::ReadModel(const Path& filepath,
                           Array<Vector3> *vertexPos,
                           Array<Vector3> *vertexNormals,
                           Array<Vector2> *vertexUvs)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filepath.GetAbsolute().ToCString(),
          aiProcess_Triangulate            |
          aiProcess_JoinIdenticalVertices  |
          aiProcess_GenSmoothNormals);

    bool ok = false;
    if (scene)
    {
        for (int mi = 0; mi < scene->mNumMeshes; ++mi)
        {
            aiMesh *mesh = scene->mMeshes[mi];
            for (int i = 0; i < mesh->mNumFaces; ++i)
            {
                const int iV0 = mesh->mFaces[i].mIndices[0];
                const int iV1 = mesh->mFaces[i].mIndices[1];
                const int iV2 = mesh->mFaces[i].mIndices[2];

                if (mesh->HasPositions())
                {
                    Vector3 v0 = AIVectorToVec3(mesh->mVertices[iV0]);
                    Vector3 v1 = AIVectorToVec3(mesh->mVertices[iV1]);
                    Vector3 v2 = AIVectorToVec3(mesh->mVertices[iV2]);
                    vertexPos->PushBack(v0);
                    vertexPos->PushBack(v1);
                    vertexPos->PushBack(v2);
                }

                if (mesh->GetNumUVChannels() > 0)
                {
                    Vector2 uv0 = AIVectorToVec3(mesh->mTextureCoords[0][iV0]).xy();
                    Vector2 uv1 = AIVectorToVec3(mesh->mTextureCoords[0][iV1]).xy();
                    Vector2 uv2 = AIVectorToVec3(mesh->mTextureCoords[0][iV2]).xy();
                    vertexUvs->PushBack(uv0);
                    vertexUvs->PushBack(uv1);
                    vertexUvs->PushBack(uv2);
                }

                if (mesh->HasNormals())
                {
                    Vector3 norm0 = AIVectorToVec3(mesh->mNormals[iV0]);
                    Vector3 norm1 = AIVectorToVec3(mesh->mNormals[iV1]);
                    Vector3 norm2 = AIVectorToVec3(mesh->mNormals[iV2]);
                    vertexNormals->PushBack(norm0);
                    vertexNormals->PushBack(norm1);
                    vertexNormals->PushBack(norm2);
                }
            }
        }
        ok = true;
    }
    else
    {
        Debug_Error("Could not load mesh " << filepath);
    }

    return ok;
}
