#include "Bang/AttrWidgetEnum.h"

#include "Bang/Array.h"

AttrWidgetEnum::AttrWidgetEnum(const XMLAttribute &xmlAttribute,
                               InspectorWidget *inspectorWidget) :
    AttributeWidget(xmlAttribute, inspectorWidget)
{
    QLayout *layout = new QHBoxLayout();
    m_layout.addLayout(layout, 1);

    m_comboBox = new ComboBox(this);
    layout->addWidget(m_comboBox);
    Array<String> enumNames = xmlAttribute.GetEnumNames();
    for (String enumString : enumNames)
    {
        m_comboBox->addItem( enumString.ToQString() );
    }
    connect(m_comboBox, SIGNAL(currentIndexChanged(QString)),
            inspectorWidget, SLOT(_OnSlotValueChanged(QString)));

    m_heightSizeHint = 45;

    AfterConstructor();
}

void AttrWidgetEnum::SetValue(int index)
{
    m_comboBox->setCurrentIndex(index);
}

int AttrWidgetEnum::GetValue()
{
    return m_comboBox->currentIndex();
}

void AttrWidgetEnum::Refresh(const XMLAttribute &attribute)
{
    AttributeWidget::Refresh(attribute);
    if (attribute.GetType() != XMLAttribute::Type::Enum) return;
    SetValue( attribute.GetEnumSelectedIndex() );
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


