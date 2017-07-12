#include "Bang/UIRendererInspectorWidget.h"

UIRendererInspectorWidget::UIRendererInspectorWidget(UIRenderer *uiRenderer)
    : RendererInspectorWidget(uiRenderer)
{
    p_uiRenderer = uiRenderer;
}

UIRendererInspectorWidget::~UIRendererInspectorWidget()
{
}

void UIRendererInspectorWidget::InitExtra()
{
    RendererInspectorWidget::InitExtra();

    m_tintAW = new AttrWidgetColor("Tint");
    m_drawWireframeAW->SetVisible(false);
    m_materialAW->SetVisible(false);

    InsertAttributeWidget(m_tintAW);
}

void UIRendererInspectorWidget::Refresh()
{
    RendererInspectorWidget::Refresh();

    m_tintAW->SetValue( p_uiRenderer->GetTint() );
}

void UIRendererInspectorWidget::OnAttrWidgetValueChanged(
        IAttributeWidget *attrWidget)
{
    RendererInspectorWidget::OnAttrWidgetValueChanged(attrWidget);

    if (attrWidget == m_tintAW)
    {
        p_uiRenderer->SetTint( m_tintAW->GetValue() );
    }
}

void UIRendererInspectorWidget::OnDestroy()
{
    RendererInspectorWidget::OnDestroy();
}
