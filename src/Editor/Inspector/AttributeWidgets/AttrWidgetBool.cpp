#include "Bang/AttrWidgetBool.h"

AttrWidgetBool::AttrWidgetBool(const String &labelText)
{
    m_checkbox = new CheckboxWidget();
    m_checkbox->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    m_horizontalLayout.addWidget(m_checkbox, 1, Qt::AlignLeft | Qt::AlignVCenter);

    QObject::connect(m_checkbox, SIGNAL(clicked(bool)),
                     this, SLOT(OnValueChanged()));

    CreateLabel(labelText);
    SetHeightSizeHint(25);
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

void AttrWidgetBool::Refresh()
{
    AttributeWidget::Refresh();
}
