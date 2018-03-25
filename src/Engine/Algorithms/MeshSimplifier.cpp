#include "Bang/MeshSimplifier.h"

#include <utility>

#include "Bang/Map.h"
#include "Bang/Set.h"
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

    if (mesh->GetVertexCount() != 1996*3) { return Array<RH<Mesh>>(); }

    Array<OctreeData> octreeData; // Retrieve all the octree data
    {
        for (int i = 0; i < mesh->GetVertexCount(); ++i)
        {
            const int vIndex = (mesh->GetVertexIndicesIBO() ?
                                        mesh->GetVertexIndices()[i] : i);
            const Vector3 &position = mesh->GetPositionsPool()[vIndex];
            octreeData.PushBack( std::make_pair(vIndex, position) );
            if (vIndex == 513)
            {
                // Debug_Log("ORIGINAL Uv: " << mesh->GetUvsPool()[vIndex]);
            }
        }
    }

    using SimplOctree = Octree<OctreeData, ClassifyPoints>;

    constexpr int MaxOctreeDepth = 12;
    constexpr float PaddingPercent = 0.05f;
    SimplOctree octree;
    AABox meshAABox = mesh->GetAABBox();
    octree.SetAABox( AABox(meshAABox.GetMin() - meshAABox.GetSize() * PaddingPercent,
                           meshAABox.GetMax() + meshAABox.GetSize() * PaddingPercent) );
    octree.Fill(octreeData, MaxOctreeDepth);

    // Compute useful connectivity info for later
    using VertexIdPair = std::pair<Mesh::VertexId, Mesh::VertexId>;
    Map<Mesh::VertexId, Set<VertexIdPair> >
                vertexIndexToTriangleOtherVerticesIndices;
    for (uint tri = 0; tri < mesh->GetNumTriangles(); ++tri)
    {
        const std::array<Mesh::VertexId, 3> triVertexIndices =
                                mesh->GetTriangleVertexIndices(tri);
        for (uint i = 0; i < 3; ++i)
        {
            const Mesh::VertexId currentVertexIndex =
                                        triVertexIndices[ ((i + 0) % 3) ];
            const Mesh::VertexId otherVertexIndex0  =
                                        triVertexIndices[ ((i + 1) % 3) ];
            const Mesh::VertexId otherVertexIndex1  =
                                        triVertexIndices[ ((i + 2) % 3) ];

            // We use minmax to have ordered pair
            // (so that order does not matter in set)
            const VertexIdPair otherTriVertexIndices =
                    std::minmax(otherVertexIndex0, otherVertexIndex1);

            vertexIndexToTriangleOtherVerticesIndices[currentVertexIndex].
                    Add(otherTriVertexIndices);
        }
    }

    // For each level of detail
    Array< RH<Mesh> > simplifiedMeshesArray;
    for (int level = 0; level <= MaxOctreeDepth; ++level)
    {
        // Debug_Log("New level: " << level << "===================");

        // Get the octree nodes at that level (and leaves pruned before)
        Array<const SimplOctree*> octreeChildrenInLevel =
                                      octree.GetChildrenAtLevel(level, true);
        // Debug_Log(i << ": " << octreeChildrenInLevel);

        struct VertexData // Simple struct to hold vertex data
        {
            Vector3 pos, normal;
            Vector2 uv;
        };

        // Get vertex clusters from octree in this level (vertex clusters are
        // just the collection of vertices at each octree node)
        using ClusterId = uint;
        using VertexCluster = Map<Mesh::VertexId, VertexData>;
        Array<VertexCluster> vertexClusters;
        Map<Mesh::VertexId, ClusterId> vertexIndexToClusterIndex;
        for (const SimplOctree *octInLevel : octreeChildrenInLevel)
        {
            VertexCluster vertexCluster;
            const Array<OctreeData> octLevelData = octInLevel->GetElementsRecursive();
            for (const OctreeData &octData : octLevelData)
            {
                const Mesh::VertexId vertexIndex = octData.first;
                if (!vertexCluster.ContainsKey(vertexIndex))
                {
                    VertexData vertexData;
                    vertexData.pos    = mesh->GetPositionsPool()[vertexIndex];
                    vertexData.normal = mesh->GetNormalsPool()[vertexIndex];
                    vertexData.uv     = mesh->GetUvsPool()[vertexIndex];

                    vertexCluster.Add(vertexIndex, vertexData);
                    vertexIndexToClusterIndex[vertexIndex] = vertexClusters.Size();
                }
            }
            vertexClusters.PushBack(vertexCluster);
        }

        /*
        if (level == MaxOctreeDepth)
        {
            for (VertexCluster &vc : vertexClusters)
            {
                if (vc.Size() > 1)
                {
                    for (auto &pair : vc)
                    {
                        VertexData &vd = pair.second;
                        vd.pos += vd.normal * 5.0f;
                    }
                    //Debug_Peek(vertexCluster.Size());
                }
            }
        }*/

        // Now actually simplify the mesh. For each cluster of vertices we will
        // extract one single vertex averaging all the components of the cluster.
        Array<Vector3> positionsLOD;
        Array<Vector3> normalsLOD;
        Array<Vector2> uvsLOD;
        RH<Mesh> simplifiedMesh = Resources::Create<Mesh>();
        for (const VertexCluster &vertexCluster : vertexClusters)
        {
            if (vertexCluster.IsEmpty()) { continue; }

            Vector3 clusterPositionsMean = Vector3::Zero;
            Vector3 clusterNormalsMean   = Vector3::Zero;
            Vector2 clusterUvsMean       = Vector2::Zero;
            for (const auto &pair : vertexCluster)
            {
                const VertexData &vData = pair.second;
                clusterPositionsMean += vData.pos;
                clusterNormalsMean   += vData.normal;
                clusterUvsMean       += vData.uv;
                // if (pair.first == 513)
                // {
                //     Debug_Peek(vData.uv);
                //     Debug_Peek(vData.normal);
                // }
            }

            const float vertexClusterSize = vertexCluster.Size();
            clusterPositionsMean /= vertexClusterSize;
            clusterNormalsMean   /= vertexClusterSize;
            clusterUvsMean       /= vertexClusterSize;
            positionsLOD.PushBack(clusterPositionsMean);
            normalsLOD.PushBack(clusterNormalsMean);
            uvsLOD.PushBack(clusterUvsMean);
        }
        simplifiedMesh.Get()->LoadPositionsPool(positionsLOD);
        simplifiedMesh.Get()->LoadNormalsPool(normalsLOD);
        simplifiedMesh.Get()->LoadUvsPool(uvsLOD);
        // Debug_Log("Level " << level << " has " << positionsLOD.Size() << "/" << mesh->GetVertexCount() << " positions");
        // Debug_Log("Level " << level << " has " << octreeChildrenInLevel.Size() << " octrees in the level.");


        // All is left is determining the face vertex indices...For this we will
        // determine which combinations of 3 vertex clusters form up triangles.
        // We know that 3 vertex clusters form a triangle if there exists a
        // triangle of the original mesh formed by one vertex of each of the 3
        // vertex clusters.

        // First: for each vertex index, know inside which vertex cluster its
        //        contained.
        // We have stored this in a variable before.

        // Second: for each vertex index, know which other two vertices form up
        //         a triangle with it. We have computed this before too

        // Third:  for each vertex cluster, create a set of the pair of clusters
        //         it forms a triangle with. We can do this with the prev info.
        using ClusterIdPair = std::pair<ClusterId, ClusterId>;
        Map<ClusterId, Set<ClusterIdPair> >
                clusterIdToOtherClusterIdsThatFormATriangleWithIt;
        auto &m = clusterIdToOtherClusterIdsThatFormATriangleWithIt;

        // For each cluster
        for (ClusterId cId = 0; cId < vertexClusters.Size(); ++cId)
        {
            // For each vertex in each vertex cluster
            const VertexCluster &vertexCluster = vertexClusters[cId];
            for (const auto &pair : vertexCluster)
            {
                Mesh::VertexId vertexInClusterIndex = pair.first;
                const ClusterId vCId =
                        vertexIndexToClusterIndex[vertexInClusterIndex];
                const Set<VertexIdPair> &otherVertexIndicesThatFormATri =
                        vertexIndexToTriangleOtherVerticesIndices.
                            Get(vertexInClusterIndex);

                // For each vertex pair that forms a tri with each vertex in each
                // vertex cluster
                for (const VertexIdPair &otherTriVertexIndices :
                                            otherVertexIndicesThatFormATri)
                {
                    // We use minmax to have ordered pair
                    // (so that order does not matter in set)
                    const VertexIdPair otherVertexIndicesThatFormATri =
                                std::minmax(otherTriVertexIndices.first,
                                            otherTriVertexIndices.second);
                    const VertexIdPair &vp = otherVertexIndicesThatFormATri;
                    const ClusterIdPair otherClusterIndicesThatFormATri =
                        std::minmax(vertexIndexToClusterIndex[vp.first],
                                    vertexIndexToClusterIndex[vp.second]);
                    m[vCId].Add(otherClusterIndicesThatFormATri);
                }
            }
        }

        // Finally calculate vertexIndices with all previous info!
        Array<Mesh::VertexId> vertexClusterIndices;
        for (ClusterId cId = 0; cId < vertexClusters.Size(); ++cId)
        {
            const Set<ClusterIdPair> &otherClusterIdsThatFormATriWithThis =
                    clusterIdToOtherClusterIdsThatFormATriangleWithIt[cId];
            for (const ClusterIdPair &otherTriVerticesClusterIdPair :
                                   otherClusterIdsThatFormATriWithThis)
            {
                const ClusterId otherCId0 = otherTriVerticesClusterIdPair.first;
                const ClusterId otherCId1 = otherTriVerticesClusterIdPair.second;
                // if (cId < otherCId0 && (otherCId0 < otherCId1) && cId < otherCId1)
                {
                    ASSERT(cId       < simplifiedMesh.Get()->GetVertexCount());
                    ASSERT(otherCId0 < simplifiedMesh.Get()->GetVertexCount());
                    ASSERT(otherCId1 < simplifiedMesh.Get()->GetVertexCount());
                    vertexClusterIndices.PushBack(cId);
                    vertexClusterIndices.PushBack(otherCId0);
                    vertexClusterIndices.PushBack(otherCId1);
                }
            }
        }
        // Debug_Log("======================================================");
        // Debug_Log("======================================================");
        // Debug_Log("======================================================");
        // Debug_Log("Original: " << mesh->GetVertexIndices());
        // Debug_Log("Level " << level << ": " << vertexClusterIndices);
        // Debug_Log("======================================================");
        // Debug_Log("======================================================");
        // Debug_Log("======================================================");
        simplifiedMesh.Get()->LoadVertexIndices(vertexClusterIndices);

        simplifiedMeshesArray.PushBack(simplifiedMesh);
    }
    return simplifiedMeshesArray;
}
