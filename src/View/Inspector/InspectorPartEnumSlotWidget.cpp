#include "InspectorPartEnumSlotWidget.h"

InspectorPartEnumSlotWidget::InspectorPartEnumSlotWidget(float initialValue,
                                                         const std::string &labelString,
                                                         InspectorPartWidget *parent) : InspectorPartSlotWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setSpacing(0); layout->setContentsMargins(0,0,0,0);
    this->setLayout(layout);

    if(labelString != "")
    {
        QLabel *textLabel = new QLabel(QString::fromStdString(labelString));
        textLabel->setContentsMargins(0,0,0,0);
        textLabel->show();
        layout->addWidget(textLabel);
    }

    comboBox = new QComboBox();
    comboBox->addItem("A");
    comboBox->addItem("B");
    comboBox->show();

    layout->addWidget(comboBox);

    this->setContentsMargins(0,0,0,0);
    this->show();
}

