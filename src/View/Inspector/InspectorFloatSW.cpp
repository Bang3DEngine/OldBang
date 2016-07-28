#include "InspectorFloatSW.h"

InspectorFloatSW::InspectorFloatSW(const std::string &labelString,
                                   float initialValue,
                                   InspectorWidget *parent,
                                   XMLNode *relatedXMLInfo) :
    InspectorSW(labelString, parent, relatedXMLInfo)
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
    m_spinbox->setValue(initialValue);
    m_spinbox->setAlignment(Qt::AlignHCenter);
    m_spinbox->setMinimum(-9999999.9);
    m_spinbox->setMaximum(9999999.9);
    m_spinbox->setAccelerated(true);
    m_spinbox->setMinimumWidth(50);
    m_spinbox->setContentsMargins(0,0,0,0);
    m_spinbox->show();

    layout->addWidget(m_spinbox);

    this->setContentsMargins(0,0,0,0);
    this->show();

    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
}

void InspectorFloatSW::SetValue(float f)
{
    if (!_editing)
    {
        disconnect(m_spinbox, SIGNAL(valueChanged(double)), m_parent, SLOT(_OnSlotValueChanged(double)));
        m_spinbox->setValue(f);
        m_spinbox->show();
        connect(m_spinbox, SIGNAL(valueChanged(double)), m_parent, SLOT(_OnSlotValueChanged(double)));
    }
}

float InspectorFloatSW::GetValue()
{
    return m_spinbox->value();
}

void InspectorFloatSW::OnSpinBoxFocusIn()
{
    _editing = true;
}

void InspectorFloatSW::OnSpinBoxFocusOut()
{
    _editing = false;
    connect(m_spinbox, SIGNAL(valueChanged(double)), m_parent, SLOT(_OnSlotValueChanged(double)));
    m_spinbox->setValue(m_spinbox->value());
    disconnect(m_spinbox, SIGNAL(valueChanged(double)), m_parent, SLOT(_OnSlotValueChanged(double)));
}

QSize InspectorFloatSW::sizeHint() const { return QSize(20, 30); }
