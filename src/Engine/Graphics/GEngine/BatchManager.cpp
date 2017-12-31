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
                               const BatchParameters &batchParams)
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
    targetBatch->AddGeometry(positions, normals, uvs);
}

void BatchManager::Render()
{
    Debug_Peek(m_currentBatches.Size());
    for (const Batch &batch : m_currentBatches)
    {
        batch.Render();
    }
}

void BatchManager::ClearBatches()
{
    m_currentBatches.Clear();
}

