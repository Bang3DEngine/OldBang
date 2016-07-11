#include "InspectorEnumSW.h"

InspectorEnumSW::InspectorEnumSW(const std::string &labelString,
                                         const std::vector<std::string> &enumValues,
                                         const int selectedValue,
                                         InspectorWidget *parent) : InspectorSW(labelString, parent)
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

    p_comboBox = new QComboBox();
    for(std::string enumString : enumValues)
    {
        p_comboBox->addItem( QString::fromStdString(enumString) );
    }
    p_comboBox->setCurrentIndex(selectedValue);
    p_comboBox->show();
    connect(p_comboBox, SIGNAL(currentIndexChanged(QString)), parent, SLOT(_OnSlotValueChanged(QString)));


    layout->addWidget(p_comboBox);

    this->setContentsMargins(0,0,0,0);
    this->show();
}

void InspectorEnumSW::SetValue(int index)
{
    p_comboBox->setCurrentIndex(index);
}

int InspectorEnumSW::GetValue()
{
    return p_comboBox->currentIndex();
}

