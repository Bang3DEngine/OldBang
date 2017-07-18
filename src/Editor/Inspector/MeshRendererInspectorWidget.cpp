#include "Bang/MeshRendererInspectorWidget.h"

#include "Bang/Mesh.h"
#include "Bang/Extensions.h"
#include "Bang/AssetsManager.h"

MeshRendererInspectorWidget::MeshRendererInspectorWidget(
        MeshRenderer *meshRenderer)
    : RendererInspectorWidget(meshRenderer)
{
    p_meshRenderer = meshRenderer;
}

MeshRendererInspectorWidget::~MeshRendererInspectorWidget()
{
}

void MeshRendererInspectorWidget::InitExtra()
{
    RendererInspectorWidget::InitExtra();

    m_meshAW = new AttrWidgetFile("Mesh", {Extensions::Get<Mesh>()});

    InsertAttributeWidget(m_meshAW);
}

void MeshRendererInspectorWidget::OnUpdate()
{
    RendererInspectorWidget::OnUpdate();

    Mesh *mesh = p_meshRenderer->GetMesh();
    Path meshPath = mesh ? mesh->GetFilepath() : Path::Empty;
    m_meshAW->SetValue( meshPath );
}

void MeshRendererInspectorWidget::OnAttrWidgetValueChanged(
        IAttributeWidget *attrWidget)
{
    RendererInspectorWidget::OnAttrWidgetValueChanged(attrWidget);

    if (attrWidget == m_meshAW)
    {
        Mesh *mesh = AssetsManager::Load<Mesh>(m_meshAW->GetPath());
        p_meshRenderer->SetMesh( mesh );
    }

    emit Changed(this);
}

void MeshRendererInspectorWidget::OnDestroy()
{
    RendererInspectorWidget::OnDestroy();
}

