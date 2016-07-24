#include "EditorFloor.h"

EditorFloor::EditorFloor()
{
    const int GridSize2 = 2 * (c_gridSize + 1);
    m_lineRenderers.resize(GridSize2);
    for (int i = 0; i < GridSize2; ++i)
    {
        m_lineRenderers[i] = AddComponent<SingleLineRenderer>();
        m_lineRenderers[i]->SetReceivesLighting(false);
        m_lineRenderers[i]->GetMaterial()->SetDiffuseColor(glm::vec4(glm::vec3(0.4f), 1));
    }
}

EditorFloor::~EditorFloor()
{

}

void EditorFloor::OnUpdate()
{
    EditorGameObject::OnUpdate();

    Camera *cam  = Canvas::GetCurrentScene()->GetCamera();
    Transform *camt = cam->gameObject->GetTransform();

    const float LineSize2 = (c_gridSize * c_tileSize) / 2.0f; // Half line length

    int indexOffsetX = int(camt->GetPosition().x / c_tileSize);
    int indexOffsetZ = int(camt->GetPosition().z / c_tileSize);

    for (int i = 0; i <= c_gridSize; ++i)
    {
        SingleLineRenderer *lr = m_lineRenderers[i];

        int j = i - c_gridSize / 2;   // [ -GridSize/2, GridSize/2 ]
        j += indexOffsetX;

        Vector3 center = Vector3(c_tileSize * j, 0, c_tileSize * indexOffsetZ);

        lr->SetOrigin ( Vector3(0, 0, -LineSize2) + center );
        lr->SetDestiny( Vector3(0, 0,  LineSize2) + center );
    }

    for (int i = 0; i <= c_gridSize; ++i)
    {
        SingleLineRenderer *lr = m_lineRenderers[i + c_gridSize + 1];

        int j = i - c_gridSize / 2;   // [ -GridSize/2, GridSize/2 ]
        j += indexOffsetZ;

        Vector3 center = Vector3(c_tileSize * indexOffsetX, 0, c_tileSize * j);

        lr->SetOrigin ( Vector3(-LineSize2, 0, 0) + center );
        lr->SetDestiny( Vector3( LineSize2, 0, 0) + center );
    }
}
