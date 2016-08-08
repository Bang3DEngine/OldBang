#include "AttrWidgetBool.h"

AttrWidgetBool::AttrWidgetBool(const XMLAttribute &xmlAttribute,
                               InspectorWidget *inspectorWidget) :
    AttributeWidget(xmlAttribute, inspectorWidget)
{
    QBoxLayout *layout = new QHBoxLayout();
    m_layout->addLayout(layout, 1);

    m_checkbox = new CheckboxWidget();
    layout->addWidget(m_checkbox, 1, Qt::AlignRight);

    connect(m_checkbox, SIGNAL(stateChanged(int)),
            inspectorWidget, SLOT(_OnSlotValueChanged(int)));

    AfterConstructor();
}

void AttrWidgetBool::SetValue(bool value)
{
    m_checkbox->setChecked(value);
}

bool AttrWidgetBool::GetValue() const
{
    return m_checkbox->isChecked();
}

void AttrWidgetBool::Refresh(const XMLAttribute &attribute)
{
    AttributeWidget::Refresh(attribute);
    if (attribute.GetType() != XMLAttribute::Type::Bool) return;
    SetValue( attribute.GetBool() );
}
