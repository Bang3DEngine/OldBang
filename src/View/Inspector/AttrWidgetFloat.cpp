#include "AttrWidgetFloat.h"

AttrWidgetFloat::AttrWidgetFloat(const std::string &labelString,
                                 InspectorWidget *parent) :
    AttributeWidget(labelString, parent)
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setSpacing(0); layout->setContentsMargins(0,0,0,0);
    this->setLayout(layout);

    if (labelString != "")
    {
        QLabel *textLabel = new QLabel(QString::fromStdString(labelString));
        textLabel->setContentsMargins(0,0,0,0);
        textLabel->show();
        layout->addWidget(textLabel);
    }

    m_spinbox = new FloatComponentSlotSpinBox();
    m_spinbox->setAlignment(Qt::AlignHCenter);
    m_spinbox->setMaximum( 9999999999999999.9f);
    m_spinbox->setMinimum(-9999999999999999.9f);
    m_spinbox->setAccelerated(true);
    m_spinbox->show();

    layout->addWidget(m_spinbox);

    setContentsMargins(1,1,1,1);
    show();

    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
}

void AttrWidgetFloat::SetValue(float f)
{
    if (!_editing)
    {
        disconnect(m_spinbox, SIGNAL(valueChanged(double)), m_parent, SLOT(_OnSlotValueChanged(double)));
        m_spinbox->setValue(f);
        m_spinbox->show();
        connect(m_spinbox, SIGNAL(valueChanged(double)), m_parent, SLOT(_OnSlotValueChanged(double)));
    }
}

float AttrWidgetFloat::GetValue()
{
    return m_spinbox->value();
}

void AttrWidgetFloat::OnSpinBoxFocusIn()
{
    _editing = true;
}

void AttrWidgetFloat::OnSpinBoxFocusOut()
{
    _editing = false;
    connect(m_spinbox, SIGNAL(valueChanged(double)), m_parent, SLOT(_OnSlotValueChanged(double)));
    m_spinbox->setValue(m_spinbox->value());
    disconnect(m_spinbox, SIGNAL(valueChanged(double)), m_parent, SLOT(_OnSlotValueChanged(double)));
}

QSize AttrWidgetFloat::sizeHint() const { return QSize(10, 20); }
