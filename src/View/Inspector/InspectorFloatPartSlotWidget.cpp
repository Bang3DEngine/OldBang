#include "InspectorFloatPartSlotWidget.h"

InspectorFloatPartSlotWidget::InspectorFloatPartSlotWidget(float initialValue,
                                                          const std::string &labelString,
                                                          InspectorPartWidget *parent) : InspectorPartSlotWidget(parent),
                                                          editing(false)
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setSpacing(0); layout->setContentsMargins(0,0,0,0);
    this->setLayout(layout);

    if(labelString != "")
    {
        QLabel *textLabel = new QLabel(QString::fromStdString(labelString));
        textLabel->setContentsMargins(0,0,0,0);
        textLabel->show();
        layout->addWidget(textLabel);
    }

    spinbox = new QDoubleSpinBox();
    spinbox->setValue(initialValue);
    spinbox->setAlignment(Qt::AlignHCenter);
    spinbox->setMinimum(-999999999.9);
    spinbox->setMaximum(999999999.9);
    spinbox->setAccelerated(true);
    spinbox->setMinimumWidth(50);
    spinbox->setContentsMargins(0,0,0,0);
    spinbox->show();
    connect(spinbox, SIGNAL(valueChanged(double)), parent, SLOT(_NotifyInspectorSlotChanged(double)));

    layout->addWidget(spinbox);

    this->setContentsMargins(0,0,0,0);
    this->show();

    lastValueBeforeEditing = GetValue();
    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
}

void InspectorFloatPartSlotWidget::SetValue(float f)
{
    if(!editing)
    {
        disconnect(spinbox, SIGNAL(valueChanged(double)), parent, SLOT(_NotifyInspectorSlotChanged(double)));
        lastValueBeforeEditing = float(spinbox->value());
        spinbox->setValue( double(f) );
        connect(spinbox, SIGNAL(valueChanged(double)), parent, SLOT(_NotifyInspectorSlotChanged(double)));
    }
}

float InspectorFloatPartSlotWidget::GetValue()
{
    return lastValueBeforeEditing;
}

void InspectorFloatPartSlotWidget::focusInEvent(QFocusEvent *event)
{
    editing = true;
    Logger_Log("IN");
}

void InspectorFloatPartSlotWidget::focusOutEvent(QFocusEvent *event)
{
    editing = false;
    Logger_Log("OUT");
}




QSize InspectorFloatPartSlotWidget::sizeHint() const { return QSize(50, 50); }
