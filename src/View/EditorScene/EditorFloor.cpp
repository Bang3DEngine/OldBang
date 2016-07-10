#include "EditorFloor.h"

EditorFloor::EditorFloor()
{
    const int GridSize2 = 2 * (GridSize + 1);
    lineRenderers.resize(GridSize2);
    for (int i = 0; i < GridSize2; ++i)
    {
        lineRenderers[i] = AddComponent<SingleLineRenderer>();
        lineRenderers[i]->GetMaterial()->SetDiffuseColor(glm::vec4(glm::vec3(0.4f), 1));
    }

    AddComponent<Transform>();
}

EditorFloor::~EditorFloor()
{

}

void EditorFloor::OnUpdate()
{
    EditorGameObject::OnUpdate();

    Camera *cam  = Canvas::GetCurrentScene()->GetCamera();
    Transform *camt = cam->gameObject->GetComponent<Transform>();

    const float LineSize2 = (GridSize * TileSize) / 2.0f; // Half line length

    int indexOffsetX = int(camt->GetPosition().x / TileSize);
    int indexOffsetZ = int(camt->GetPosition().z / TileSize);

    for (int i = 0; i <= GridSize; ++i)
    {
        SingleLineRenderer *lr = lineRenderers[i];

        int j = i - GridSize / 2;   // [ -GridSize/2, GridSize/2 ]
        j += indexOffsetX;

        Vector3 center = Vector3(TileSize * j, 0, TileSize * indexOffsetZ);

        lr->SetOrigin ( Vector3(0, 0, -LineSize2) + center );
        lr->SetDestiny( Vector3(0, 0,  LineSize2) + center );
    }

    for (int i = 0; i <= GridSize; ++i)
    {
        SingleLineRenderer *lr = lineRenderers[i + GridSize + 1];

        int j = i - GridSize / 2;   // [ -GridSize/2, GridSize/2 ]
        j += indexOffsetZ;

        Vector3 center = Vector3(TileSize * indexOffsetX, 0, TileSize * j);

        lr->SetOrigin ( Vector3(-LineSize2, 0, 0) + center );
        lr->SetDestiny( Vector3( LineSize2, 0, 0) + center );
    }
}
