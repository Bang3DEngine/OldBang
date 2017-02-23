#include "EditorBBox.h"

#include "Debug.h"
#include "Mesh.h"
#include "Material.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "AssetsManager.h"

Material *EditorBBox::s_boxMaterial = nullptr;

EditorBBox::EditorBBox(GameObject *attachedGameObject) : GameObject("EditorBBox")
{
    this->m_attachedGameObject = attachedGameObject;

    MeshRenderer *mr = AddComponent<MeshRenderer>();

    // Copy the lines material to the box material, and save it in cache
    // only the first time. The rest of the times, load it from cache
    Material *linesMaterial =
            AssetsManager::Load<Material>("./Materials/G_Default.bmat", true);

    if (!s_boxMaterial)
    {
        s_boxMaterial = new Material(*linesMaterial);
    }

    s_boxMaterial->SetDiffuseColor(Color::Green);
    mr->SetMaterial(s_boxMaterial);

    //Create the box mesh, and save it to cache
    // only the first time. The rest of the times, load it from cache.
    Mesh *mesh = AssetsManager::Load<Mesh>("./Meshes/Cube.bmesh", true);

    mr->SetMesh(mesh);
    mr->SetDrawWireframe(true);
    mr->SetCullMode(GL::CullMode::Back);
    mr->SetRenderMode(GL::RenderMode::Triangles);

    AddHideFlag(HideFlags::HideAndDontSave);
}

void EditorBBox::OnUpdate()
{
    ASSERT(m_attachedGameObject);

    // Adjust transform to wrap all the vertices of the parent and children
    AABox bbox;
    List<Renderer*> rends = m_attachedGameObject->GetComponents<Renderer>();
    for (auto it_r = rends.Begin(); it_r != rends.End(); ++it_r)
    {
        Renderer *r = *it_r;
        if (CAN_USE_COMPONENT(r))
        {
            AABox mbox = r->GetAABBox();
            bbox = AABox::Union(bbox, mbox);
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


Array<Vector3> EditorBBox::s_boxVertices =
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
