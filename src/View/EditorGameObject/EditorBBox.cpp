#include "EditorBBox.h"

EditorBBox::EditorBBox() : EditorGameObject("EditorBBox")
{
    AddComponent<Transform>();

    MeshRenderer *mr = AddComponent<MeshRenderer>();

    //Copy the lines material to the box material, and save it in cache
    // only the first time. The rest of the times, load it from cache
    Material *linesMaterial =
            AssetsManager::GetAsset<Material>("res/Assets/linesMaterial.bmat");
    Material *boxMaterial = nullptr;
    if(!AssetsManager::ExistsAssetInCache("EditorBBox_Mat"))
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
    if(!AssetsManager::ExistsAssetInCache("EditorBBox_Mesh"))
    {
        mesh = new Mesh();
        mesh->LoadPositions(boxVertices);
    }
    else
    {
        mesh = AssetsManager::GetRuntimeAsset<Mesh>("EditorBBox_Mesh");
    }

    mr->SetMesh(mesh);
    mr->SetCullMode(Renderer::CullMode::None);
    mr->SetDrawWireframe(true);
    mr->SetRenderMode(Renderer::RenderMode::Triangles);
}

void EditorBBox::OnUpdate()
{
    GameObject *parent = this;
    while(parent->IsEditorGameObject() && !parent->IsScene())
    { //Find the closest nonEditor parent, stop at top scene
        parent = parent->GetParent();
    }

    if(parent != nullptr)
    {
        //Adjust transform to wrap all the vertices of the parent and children
        Box bbox;
        std::list<MeshRenderer*> mrs =
                //parent->GetComponentsInThisAndChildren<MeshRenderer>();
                parent->GetComponents<MeshRenderer>();

        for(auto it_mr = mrs.begin(); it_mr != mrs.end(); ++it_mr)
        {
            MeshRenderer *mr = *it_mr;
            if(CAN_USE_COMPONENT(mr))
            {
                const Mesh *m = mr->GetMesh();
                if(m->GetVertexCount() > 0)
                {
                    Box mbox = m->GetBoundingBox();
                    bbox = Box::Union(bbox, mbox);
                }
            }
        }

        Transform *t = GetComponent<Transform>();

        Vector3 center = bbox.GetCenter();
        t->SetPosition(center);

        Vector3 scale = Vector3(bbox.GetWidth(),
                                bbox.GetHeight(),
                                bbox.GetDepth());
        t->SetScale(scale);
    }
}


std::vector<Vector3> EditorBBox::boxVertices =
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
