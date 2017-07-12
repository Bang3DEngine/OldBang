#include "Bang/AttrWidgetEnum.h"

#include "Bang/Array.h"

AttrWidgetEnum::AttrWidgetEnum(const XMLAttribute &xmlAttribute) :
    AttributeWidget(xmlAttribute)
{
    QLayout *layout = new QHBoxLayout();
    m_horizontalLayout.addLayout(layout, 1);

    m_comboBox = new ComboBox(this);
    layout->addWidget(m_comboBox);
    Array<String> enumNames = xmlAttribute.GetEnumNames();
    for (String enumString : enumNames)
    {
        m_comboBox->addItem( enumString.ToQString() );
    }
    QObject::connect(m_comboBox, SIGNAL(currentIndexChanged(QString)),
                     this, SIGNAL(OnValueChanged()));

    SetHeightSizeHint(35);

    AfterConstructor();
}

void AttrWidgetEnum::SetValue(int index)
{
    m_comboBox->blockSignals(true);
    m_comboBox->setCurrentIndex(index);
    m_comboBox->blockSignals(false);
}

int AttrWidgetEnum::GetValue() const
{
    return m_comboBox->currentIndex();
}

void AttrWidgetEnum::Refresh(const XMLAttribute &attribute)
{
    AttributeWidget::Refresh(attribute);
    if (attribute.GetType() != XMLAttribute::Type::Enum) return;
    SetValue( attribute.GetEnumSelectedIndex() );
}

XMLAttribute AttrWidgetEnum::GetXMLAttribute() const
{
    Array<String> enumNames;
    for (int i = 0; i < m_comboBox->children().size(); ++i)
    {
        enumNames.PushBack(m_comboBox->itemText(i));
    }

    XMLAttribute attr;
    attr.SetEnum(enumNames, GetValue());
    return attr;
}

ComboBox::ComboBox(QWidget *parent)
    : QComboBox(parent)
{
    setFocusPolicy(Qt::FocusPolicy::StrongFocus);
}

void ComboBox::wheelEvent(QWheelEvent *event)
{
    if (hasFocus())
    {
        QComboBox::wheelEvent(event);
    }
    else
    {
        event->ignore();
    }
}


