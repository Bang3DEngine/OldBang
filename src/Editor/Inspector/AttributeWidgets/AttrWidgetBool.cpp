#include "Bang/AttrWidgetBool.h"

AttrWidgetBool::AttrWidgetBool(const XMLAttribute &xmlAttribute) :
    AttributeWidget(xmlAttribute)
{
    m_checkbox = new CheckboxWidget();
    m_checkbox->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    m_horizontalLayout.addWidget(m_checkbox, 1, Qt::AlignLeft | Qt::AlignVCenter);

    QObject::connect(m_checkbox, SIGNAL(clicked(bool)),
                     this, SIGNAL(OnValueChanged()));

    setFixedHeight(25);
    AfterConstructor();
}

void AttrWidgetBool::SetValue(bool value)
{
    blockSignals(true);
    m_checkbox->setChecked(value);
    blockSignals(false);
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
