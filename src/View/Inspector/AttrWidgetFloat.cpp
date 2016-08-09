#include "AttrWidgetFloat.h"

AttrWidgetFloat::AttrWidgetFloat(const XMLAttribute &xmlAttribute,
                                 InspectorWidget *inspectorWidget,
                                 bool isSubWidget) :
    AttributeWidget(xmlAttribute, inspectorWidget, isSubWidget, !isSubWidget)
{
    QVBoxLayout *layout = new QVBoxLayout();
    m_layout->addLayout(layout, 1);
    m_layout->setMargin(1);

    m_spinbox = new FloatComponentSlotSpinBox();
    m_spinbox->setAlignment(Qt::AlignLeft);
    m_spinbox->setMaximum( 9999999999999999.9f);
    m_spinbox->setMinimum(-9999999999999999.9f);
    m_spinbox->setAccelerated(true);
    m_spinbox->adjustSize();
    m_spinbox->show();

    layout->addWidget(m_spinbox);

    setMinimumWidth(25);
    setMinimumHeight(25);
    setMaximumHeight(25);
    setContentsMargins(0, 0, 0, 0);
    m_spinbox->updateGeometry();

    AfterConstructor();
}

void AttrWidgetFloat::SetValue(float f)
{
    if (!_editing)
    {
        disconnect(m_spinbox, SIGNAL(valueChanged(double)),
                   m_inspectorWidget, SLOT(_OnSlotValueChanged(double)));
        m_spinbox->setValue(f);
        m_spinbox->show();
        connect(m_spinbox, SIGNAL(valueChanged(double)),
                m_inspectorWidget, SLOT(_OnSlotValueChanged(double)));
    }
}

float AttrWidgetFloat::GetValue()
{
    return m_spinbox->value();
}

void AttrWidgetFloat::Refresh(const XMLAttribute &attribute)
{
    AttributeWidget::Refresh(attribute);

    if (attribute.GetType() != XMLAttribute::Type::Float) return;
    SetValue( attribute.GetFloat() );
}

void AttrWidgetFloat::OnSpinBoxFocusIn()
{
    _editing = true;
}

void AttrWidgetFloat::OnSpinBoxFocusOut()
{
    _editing = false;
    connect(m_spinbox, SIGNAL(valueChanged(double)),
            m_inspectorWidget, SLOT(_OnSlotValueChanged(double)));
    m_spinbox->setValue(m_spinbox->value());
    disconnect(m_spinbox, SIGNAL(valueChanged(double)),
               m_inspectorWidget, SLOT(_OnSlotValueChanged(double)));
}


