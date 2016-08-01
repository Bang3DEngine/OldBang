#include "AttrWidgetBool.h"

AttrWidgetBool::AttrWidgetBool(const std::string &labelString,
                               InspectorWidget *parent) :
    AttributeWidget(labelString, parent)
{
    QLayout *layout = new QHBoxLayout();
    setLayout(layout);

    m_checkbox = new CheckboxWidget();
    m_checkbox->setContentsMargins(0,0,0,0);
    m_checkbox->show();

    layout->addWidget(GetLabelWidget(labelString));
    layout->addWidget(m_checkbox);

    connect(m_checkbox, SIGNAL(stateChanged(int)),
            parent, SLOT(_OnSlotValueChanged(int)));

    setContentsMargins(0,0,0,0);
    show();

    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
}

void AttrWidgetBool::SetValue(bool value)
{
    m_checkbox->setChecked(value);
}

bool AttrWidgetBool::GetValue() const
{
    return m_checkbox->isChecked();
}
