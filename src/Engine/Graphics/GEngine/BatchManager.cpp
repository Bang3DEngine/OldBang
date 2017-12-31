#include "Bang/BatchManager.h"

USING_NAMESPACE_BANG

BatchManager::BatchManager()
{
}

BatchManager::~BatchManager()
{
}

void BatchManager::AddGeometry(const Array<Vector3> &positions,
                               const Array<Vector3> &normals,
                               const Array<Vector2> &uvs,
                               const BatchParameters &batchParams,
                               bool incrementalZ)
{
    Batch *targetBatch = nullptr;
    for (Batch &batch : m_currentBatches)
    {
        if ( batch.GetParameters().Equals(batchParams) )
        {
            targetBatch = &batch;
        }
    }

    if (!targetBatch)
    {
        m_currentBatches.PushBack( Batch(batchParams) );
        targetBatch = &m_currentBatches.Back();
    }
    ASSERT(targetBatch);

    Array<Vector3> finalPositions;
    if (incrementalZ)
    {
        m_incrementalZ -= 0.00001;
        for (const Vector3 &pos : positions)
        {
            finalPositions.PushBack( pos + Vector3(0, 0, m_incrementalZ) );
        }
    }
    else { finalPositions = positions; }

    targetBatch->AddGeometry(finalPositions, normals, uvs);
}

void BatchManager::Render()
{
    Debug_Log("Rendering " << m_currentBatches.Size() << " batches");
    for (const Batch &batch : m_currentBatches)
    {
        batch.Render();
    }
}

void BatchManager::ClearBatches()
{
    m_incrementalZ = 0.0;
    m_currentBatches.Clear();
}

