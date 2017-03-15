#include "AttrWidgetBool.h"

AttrWidgetBool::AttrWidgetBool(const XMLAttribute &xmlAttribute,
                               InspectorWidget *inspectorWidget) :
    AttributeWidget(xmlAttribute, inspectorWidget)
{
    m_checkbox = new CheckboxWidget();
    m_checkbox->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    m_layout.addWidget(m_checkbox, 1, Qt::AlignLeft | Qt::AlignVCenter);

    connect(m_checkbox, SIGNAL(stateChanged(int)),
            inspectorWidget, SLOT(_OnSlotValueChanged(int)));

    setFixedHeight(25);
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
