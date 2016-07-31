#include "AttrWidgetEnum.h"

AttrWidgetEnum::AttrWidgetEnum(const std::string &labelString,
                               const std::vector<std::string> &enumNames,
                               InspectorWidget *parent) :
    AttributeWidget(labelString, parent)
{
    QHBoxLayout *layout = new QHBoxLayout();
    setLayout(layout);

    m_comboBox = new QComboBox();
    for (std::string enumString : enumNames)
    {
        m_comboBox->addItem( QString::fromStdString(enumString) );
    }
    m_comboBox->show();
    connect(m_comboBox, SIGNAL(currentIndexChanged(QString)), parent, SLOT(_OnSlotValueChanged(QString)));

    layout->addWidget(GetLabelWidget(labelString));
    layout->addWidget(m_comboBox);

    show();
}

void AttrWidgetEnum::SetValue(int index)
{
    m_comboBox->setCurrentIndex(index);
}

int AttrWidgetEnum::GetValue()
{
    return m_comboBox->currentIndex();
}

