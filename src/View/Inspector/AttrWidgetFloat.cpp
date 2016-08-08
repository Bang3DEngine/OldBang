#include "AttrWidgetFloat.h"

AttrWidgetFloat::AttrWidgetFloat(const XMLAttribute &xmlAttribute,
                                 InspectorWidget *inspectorWidget) :
    AttributeWidget(xmlAttribute, inspectorWidget, false)
{
    QVBoxLayout *layout = new QVBoxLayout();
    m_layout->addLayout(layout, 1);
    layout->setMargin(0);

    m_spinbox = new FloatComponentSlotSpinBox();
    m_spinbox->setAlignment(Qt::AlignHCenter);
    m_spinbox->setMaximum( 9999999999999999.9f);
    m_spinbox->setMinimum(-9999999999999999.9f);
    m_spinbox->setAccelerated(true);
    m_spinbox->adjustSize();
    m_spinbox->show();

    layout->addWidget(m_spinbox);

    AfterConstructor();
}

void AttrWidgetFloat::SetValue(float f)
{
    if (!_editing)
    {
        disconnect(m_spinbox, SIGNAL(valueChanged(double)), m_inspectorWidget, SLOT(_OnSlotValueChanged(double)));
        m_spinbox->setValue(f);
        m_spinbox->show();
        connect(m_spinbox, SIGNAL(valueChanged(double)), m_inspectorWidget, SLOT(_OnSlotValueChanged(double)));
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
    connect(m_spinbox, SIGNAL(valueChanged(double)), m_inspectorWidget, SLOT(_OnSlotValueChanged(double)));
    m_spinbox->setValue(m_spinbox->value());
    disconnect(m_spinbox, SIGNAL(valueChanged(double)), m_inspectorWidget, SLOT(_OnSlotValueChanged(double)));
}

QSize AttrWidgetFloat::sizeHint() const { return QSize(10, 20); }
