#include "EditorBBox.h"

EditorBBox::EditorBBox(GameObject *attachedGameObject) : EditorGameObject("EditorBBox")
{
    this->m_attachedGameObject = attachedGameObject;

    MeshRenderer *mr = AddComponent<MeshRenderer>();

    // Copy the lines material to the box material, and save it in cache
    // only the first time. The rest of the times, load it from cache
    Material *linesMaterial =
            AssetsManager::GetAsset<Material>("Assets/Engine/Materials/D2G_Line.bmat");
    Material *boxMaterial = nullptr;
    if (!AssetsManager::ExistsAssetInCache("EditorBBox_Mat"))
    {
        boxMaterial = new Material(*linesMaterial);
        boxMaterial->SetDiffuseColor(glm::vec4(0,1,0,1));
    }
    else
    {
        boxMaterial =
                AssetsManager::GetRuntimeAsset<Material>("EditorBBox_Mat");
    }
    mr->SetMaterial(boxMaterial);

    //Create the box mesh, and save it to cache
    // only the first time. The rest of the times, load it from cache.
    Mesh *mesh = nullptr;
    if (!AssetsManager::ExistsAssetInCache("EditorBBox_Mesh"))
    {
        mesh = new Mesh();
        mesh->LoadPositions(s_boxVertices);
    }
    else
    {
        mesh = AssetsManager::GetRuntimeAsset<Mesh>("EditorBBox_Mesh");
    }

    mr->SetMesh(mesh);
    mr->SetDrawWireframe(true);
    mr->SetCullMode(Renderer::CullMode::Back);
    mr->SetRenderMode(Renderer::RenderMode::Triangles);
}

void EditorBBox::OnUpdate()
{
    NONULL(m_attachedGameObject);

    // Adjust transform to wrap all the vertices of the parent and children
    Box bbox;
    std::list<Renderer*> rends = m_attachedGameObject->GetComponents<Renderer>();
    for (auto it_r = rends.begin(); it_r != rends.end(); ++it_r)
    {
        Renderer *r = *it_r;
        if (CAN_USE_COMPONENT(r))
        {
            Box mbox = r->GetBoundingBox();
            bbox = Box::Union(bbox, mbox);
        }
    }

    Transform *t = transform;

    Vector3 center = bbox.GetCenter();
    t->SetLocalPosition(center);

    Vector3 scale = Vector3(bbox.GetWidth(),
                            bbox.GetHeight(),
                            bbox.GetDepth());
    t->SetLocalScale(scale);
}


std::vector<Vector3> EditorBBox::s_boxVertices =
{
    Vector3( 0.5f, 0.5f, 0.5f),
    Vector3( 0.5f,-0.5f,-0.5f),
    Vector3( 0.5f, 0.5f,-0.5f),

    Vector3( 0.5f,-0.5f,-0.5f),
    Vector3( 0.5f, 0.5f, 0.5f),
    Vector3( 0.5f,-0.5f, 0.5f),

    Vector3(-0.5f,-0.5f,-0.5f),
    Vector3(-0.5f,-0.5f, 0.5f),
    Vector3(-0.5f, 0.5f, 0.5f),

    Vector3(-0.5f,-0.5f,-0.5f),
    Vector3(-0.5f, 0.5f, 0.5f),
    Vector3(-0.5f, 0.5f,-0.5f),



    Vector3( 0.5f, 0.5f, 0.5f),
    Vector3( 0.5f, 0.5f,-0.5f),
    Vector3(-0.5f, 0.5f,-0.5f),

    Vector3( 0.5f, 0.5f, 0.5f),
    Vector3(-0.5f, 0.5f,-0.5f),
    Vector3(-0.5f, 0.5f, 0.5f),

    Vector3( 0.5f,-0.5f, 0.5f),
    Vector3(-0.5f,-0.5f,-0.5f),
    Vector3( 0.5f,-0.5f,-0.5f),

    Vector3( 0.5f,-0.5f, 0.5f),
    Vector3(-0.5f,-0.5f, 0.5f),
    Vector3(-0.5f,-0.5f,-0.5f),



    Vector3(-0.5f, 0.5f, 0.5f),
    Vector3(-0.5f,-0.5f, 0.5f),
    Vector3( 0.5f,-0.5f, 0.5f),

    Vector3( 0.5f, 0.5f, 0.5f),
    Vector3(-0.5f, 0.5f, 0.5f),
    Vector3( 0.5f,-0.5f, 0.5f),

    Vector3( 0.5f, 0.5f,-0.5f),
    Vector3(-0.5f,-0.5f,-0.5f),
    Vector3(-0.5f, 0.5f,-0.5f),

    Vector3( 0.5f, 0.5f,-0.5f),
    Vector3( 0.5f,-0.5f,-0.5f),
    Vector3(-0.5f,-0.5f,-0.5f)
};
