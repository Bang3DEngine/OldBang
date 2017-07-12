#include "Bang/RendererInspectorWidget.h"

#include "Bang/Material.h"
#include "Bang/AssetsManager.h"

RendererInspectorWidget::RendererInspectorWidget(Renderer *renderer)
    : ComponentWidget(renderer)
{
    p_renderer = renderer;
}

RendererInspectorWidget::~RendererInspectorWidget()
{
}

void RendererInspectorWidget::InitExtra()
{
    ComponentWidget::InitExtra();

    m_materialAW      = new AttrWidgetFile("Material");
    m_drawWireframeAW = new AttrWidgetBool("Wireframe");
    m_lineWidthAW     = new AttrWidgetFloat("Wire Width");

    InsertAttributeWidget(m_materialAW);
    InsertAttributeWidget(m_drawWireframeAW);
    InsertAttributeWidget(m_lineWidthAW);
}

void RendererInspectorWidget::Refresh()
{
    ComponentWidget::Refresh();

    m_materialAW->SetValue( p_renderer->GetMaterial()->GetFilepath() );
    m_drawWireframeAW->SetValue( p_renderer->GetDrawWireframe() );
    m_lineWidthAW->SetValue( p_renderer->GetLineWidth() );
}

void RendererInspectorWidget::OnAttrWidgetValueChanged(IAttributeWidget *attrWidget)
{
    if (attrWidget == m_materialAW)
    {
        Material *mat = AssetsManager::Load<Material>(m_materialAW->GetPath());
        p_renderer->SetMaterial(mat);
    }
    else if (attrWidget == m_drawWireframeAW)
    {
        p_renderer->SetDrawWireframe( m_drawWireframeAW->GetValue() );
    }
    else if (attrWidget == m_lineWidthAW)
    {
        p_renderer->SetLineWidth( m_lineWidthAW->GetValue() );
    }
}

void RendererInspectorWidget::OnDestroy()
{
    ComponentWidget::OnDestroy();
}
