#include "InspectorEnumSW.h"

InspectorEnumSW::InspectorEnumSW(const std::string &labelString,
                                 const std::vector<std::string> &enumValues,
                                 const int selectedValue,
                                 InspectorWidget *parent,
                                 XMLNode *relatedXMLInfo) : InspectorSW(labelString, parent, relatedXMLInfo)
{
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setSpacing(0); layout->setContentsMargins(0,0,0,0);
    this->setLayout(layout);

    if (labelString != "")
    {
        QLabel *textLabel = new QLabel(QString::fromStdString(labelString));
        textLabel->setContentsMargins(0,0,0,0);
        textLabel->show();
        layout->addWidget(textLabel);
    }

    m_comboBox = new QComboBox();
    for (std::string enumString : enumValues)
    {
        m_comboBox->addItem( QString::fromStdString(enumString) );
    }
    m_comboBox->setCurrentIndex(selectedValue);
    m_comboBox->show();
    connect(m_comboBox, SIGNAL(currentIndexChanged(QString)), parent, SLOT(_OnSlotValueChanged(QString)));


    layout->addWidget(m_comboBox);

    this->setContentsMargins(0,0,0,0);
    this->show();
}

void InspectorEnumSW::SetValue(int index)
{
    m_comboBox->setCurrentIndex(index);
}

int InspectorEnumSW::GetValue()
{
    return m_comboBox->currentIndex();
}

