#include "InspectorPartEnumSlotWidget.h"

InspectorPartSlotWidget<std::vector<std::string> >::InspectorPartSlotWidget<std::vector<std::string> >
    (InspectorPartWidget *parent, const std::string &label, const std::vector<std::string> &value, int selectedValue)
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
    for(std::string enumString : enumValues)
    {
        comboBox->addItem( QString::fromStdString(enumString) );
    }
    comboBox->setCurrentIndex(selectedValue);
    comboBox->show();
    connect(comboBox, SIGNAL(currentIndexChanged(QString)), parent, SLOT(_NotifyInspectorSlotChanged(QString)));


    layout->addWidget(comboBox);

    this->setContentsMargins(0,0,0,0);
    this->show();
}
