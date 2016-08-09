#include "AttrWidgetFloat.h"

AttrWidgetFloat::AttrWidgetFloat(const XMLAttribute &xmlAttribute,
                                 InspectorWidget *inspectorWidget,
                                 bool isSubWidget) :
    AttributeWidget(xmlAttribute, inspectorWidget, isSubWidget, !isSubWidget)
{
    QVBoxLayout *layout = new QVBoxLayout();
    m_layout->addLayout(layout, 1);
    m_layout->setMargin(1);

    m_lineEdit = new FloatComponentSlotSpinBox();
    m_lineEdit->setAlignment(Qt::AlignLeft);
    // m_spinbox->setMaximum( 9999999999999999.9f);
    // m_spinbox->setMinimum(-9999999999999999.9f);

    layout->addWidget(m_lineEdit);

    setMinimumWidth(15);
    setContentsMargins(0, 0, 0, 0);
    m_lineEdit->updateGeometry();

    AfterConstructor();
}

void AttrWidgetFloat::SetValue(float f)
{
    if (!_editing)
    {
        disconnect(m_lineEdit, SIGNAL(textChanged(QString)),
                   m_inspectorWidget, SLOT(_OnSlotValueChanged(QString)));
        m_lineEdit->SetFloat(f);
        connect(m_lineEdit, SIGNAL(textChanged(QString)),
                m_inspectorWidget, SLOT(_OnSlotValueChanged(QString)));
    }
}

float AttrWidgetFloat::GetValue()
{
    return m_lineEdit->GetFloat();
}

void AttrWidgetFloat::Refresh(const XMLAttribute &attribute)
{
    AttributeWidget::Refresh(attribute);

    if (attribute.GetType() != XMLAttribute::Type::Float) return;
    SetValue( attribute.GetFloat() );
}

void AttrWidgetFloat::OnLineEditFocusIn()
{
    _editing = true;
}

void AttrWidgetFloat::OnLineEditFocusOut()
{
    _editing = false;
}


