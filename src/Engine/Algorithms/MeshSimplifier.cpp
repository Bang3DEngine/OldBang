#include "Bang/MeshSimplifier.h"

#include <utility>

#include "Bang/Mesh.h"
#include "Bang/Debug.h"
#include "Bang/Octree.h"
#include "Bang/Resources.h"
#include "Bang/DebugRenderer.h"

USING_NAMESPACE_BANG

using OctreeData = std::pair<Mesh::VertexId, Vector3>;
struct ClassifyPoints
{
    bool operator()(const AABox &aaBox, const OctreeData &data)
    {
        return aaBox.Contains(data.second);
    }
};

Array<RH<Mesh>> MeshSimplifier::GetAllMeshLODs(const Mesh *mesh)
{
    if (!mesh) { return Array<RH<Mesh>>(); }

    Array<OctreeData> positionsAndIndices;
    for (int i = 0; i < mesh->GetVertexCount(); ++i)
    {
        int vIndex = (mesh->GetVertexIndicesIBO() ? mesh->GetVertexIndices()[i] : i);
        Vector3 position = mesh->GetPositionsPool()[vIndex];
        positionsAndIndices.PushBack( std::make_pair(vIndex, position) );
    }

    using SimplOctree = Octree<OctreeData, ClassifyPoints>;

    constexpr int MaxOctreeDepth = 12;
    SimplOctree octree;
    octree.SetAABox( mesh->GetAABBox() );
    octree.Fill(positionsAndIndices, MaxOctreeDepth);

    Array< RH<Mesh> > simplifiedMeshesArray;
    for (int level = 0; level <= MaxOctreeDepth; ++level)
    {
        Array<const SimplOctree*> octreeChildrenInLevel =
                                      octree.GetChildrenAtLevel(level, true);
        // Debug_Log(i << ": " << octreeChildrenInLevel);

        struct VertexData { Vector3 pos, normal; Vector2 uv; };

        // Get vertex clusters from octree in this level
        Array< Array<VertexData> > vertexClusters;
        for (const SimplOctree *octInLevel : octreeChildrenInLevel)
        {
            Array<VertexData> vertexCluster;
            Array<OctreeData> octLevelData = octInLevel->GetElementsRecursive();
            for (const OctreeData &octData : octLevelData)
            {
                VertexData vertexData;
                vertexData.pos    = mesh->GetPositionsPool()[octData.first];
                vertexData.normal = mesh->GetNormalsPool()[octData.first];
                vertexData.uv     = mesh->GetUvsPool()[octData.first];
                vertexCluster.PushBack(vertexData);
            }
            vertexClusters.PushBack(vertexCluster);
        }

        Array<Vector3> positionsLOD;
        Array<Vector3> normalsLOD;
        Array<Vector2> uvsLOD;
        RH<Mesh> simplifiedMesh = Resources::Create<Mesh>();
        for (const Array<VertexData> &vertexCluster : vertexClusters)
        {
            Vector3 clusterPositionsMean = Vector3::Zero;
            Vector3 clusterNormalsMean   = Vector3::Zero;
            Vector2 clusterUvsMean       = Vector2::Zero;
            for (const VertexData &vData : vertexCluster)
            {
                clusterPositionsMean += vData.pos;
                clusterNormalsMean   += vData.normal;
                clusterUvsMean       += vData.uv;
            }

            const float vertexClusterSize = Math::Max(vertexCluster.Size(), 1u);
            clusterPositionsMean /= vertexClusterSize;
            clusterNormalsMean   /= vertexClusterSize;
            clusterUvsMean       /= vertexClusterSize;
            positionsLOD.PushBack(clusterPositionsMean);
            normalsLOD.PushBack(clusterNormalsMean);
            uvsLOD.PushBack(clusterUvsMean);

            if (level == 0)
            {
                // Debug_Log(clusterPositionsMean);
                // DebugRenderer::RenderPoint(clusterPositionsMean,
                //                            Color::Red, 5.0f, 2.0f, false);
            }
        }
        // Debug_Log("Level " << level << " has " << positionsLOD.Size() << "/" << mesh->GetVertexCount() << " positions");
        // Debug_Log("Level " << level << " has " << octreeChildrenInLevel.Size() << " octrees in the level.");
        simplifiedMesh.Get()->LoadPositionsPool(positionsLOD);
        simplifiedMesh.Get()->LoadNormalsPool(normalsLOD);
        simplifiedMesh.Get()->LoadUvsPool(uvsLOD);

        simplifiedMeshesArray.PushBack(simplifiedMesh);
    }
    return simplifiedMeshesArray;
}
