#include "Bang/RectTransformInspectorWidget.h"

RectTransformInspectorWidget::RectTransformInspectorWidget(RectTransform *rectTransform)
    : ComponentWidget(rectTransform)
{
    p_rectTransform = rectTransform;
}

RectTransformInspectorWidget::~RectTransformInspectorWidget()
{
}

void RectTransformInspectorWidget::InitExtra()
{
    ComponentWidget::InitExtra();

    m_marginLeftAW  = new AttrWidgetInt("Margin Left");
    m_marginTopAW   = new AttrWidgetInt("Margin Top");
    m_marginRightAW = new AttrWidgetInt("Margin Right");
    m_marginBotAW   = new AttrWidgetInt("Margin Bot");

    m_pivotPositionAW = new AttrWidgetVectorFloat("Pivot Position", 2);
    m_anchorMinAW     = new AttrWidgetVectorFloat("Anchor Min", 2);
    m_anchorMaxAW     = new AttrWidgetVectorFloat("Anchor Max", 2);

    InsertAttributeWidget(m_marginLeftAW);
    InsertAttributeWidget(m_marginTopAW);
    InsertAttributeWidget(m_marginRightAW);
    InsertAttributeWidget(m_marginBotAW);
    InsertAttributeWidget(m_pivotPositionAW);
    InsertAttributeWidget(m_anchorMinAW);
    InsertAttributeWidget(m_anchorMaxAW);

    SetEnabledCheckboxVisible(false);
}

void RectTransformInspectorWidget::OnUpdate()
{
    ComponentWidget::OnUpdate();

    m_marginLeftAW->SetValue( p_rectTransform->GetMarginLeft() );
    m_marginTopAW->SetValue( p_rectTransform->GetMarginTop() );
    m_marginRightAW->SetValue( p_rectTransform->GetMarginRight() );
    m_marginBotAW->SetValue( p_rectTransform->GetMarginBot() );

    m_pivotPositionAW->SetValue( p_rectTransform->GetPivotPosition() );
    m_anchorMinAW->SetValue( p_rectTransform->GetAnchorMin() );
    m_anchorMaxAW->SetValue( p_rectTransform->GetAnchorMax() );
}

void RectTransformInspectorWidget::OnAttrWidgetValueChanged(
        IAttributeWidget *attrWidget)
{
    ComponentWidget::OnAttrWidgetValueChanged(attrWidget);

    if (attrWidget == m_marginLeftAW)
    {
        p_rectTransform->SetMarginLeft( m_marginLeftAW->GetValue() );
    }
    else if (attrWidget == m_marginTopAW)
    {
        p_rectTransform->SetMarginTop( m_marginTopAW->GetValue() );
    }
    else if (attrWidget == m_marginRightAW)
    {
        p_rectTransform->SetMarginRight( m_marginRightAW->GetValue() );
    }
    else if (attrWidget == m_marginBotAW)
    {
        p_rectTransform->SetMarginBot( m_marginBotAW->GetValue() );
    }
    else if (attrWidget == m_pivotPositionAW)
    {
        p_rectTransform->SetPivotPosition( m_pivotPositionAW->GetVector2() );
    }
    else if (attrWidget == m_anchorMinAW)
    {
        p_rectTransform->SetAnchorMin( m_anchorMinAW->GetVector2() );
    }
    else if (attrWidget == m_anchorMaxAW)
    {
        p_rectTransform->SetAnchorMax( m_anchorMaxAW->GetVector2() );
    }

    emit Changed(this);
}

void RectTransformInspectorWidget::OnDestroy()
{
    ComponentWidget::OnDestroy();
}
