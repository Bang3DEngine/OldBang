#include "AttrWidgetButton.h"

AttrWidgetButton::AttrWidgetButton(const XMLAttribute &xmlAttribute,
                                   InspectorWidget *inspectorWidget) :
    AttributeWidget(xmlAttribute, inspectorWidget, false, false)
{
    QLayout *layout = new QVBoxLayout();
    m_layout->addLayout(layout, 1);

    m_listener = xmlAttribute.GetButtonListener();

    String buttonText = StringUtils::FormatInspectorLabel(xmlAttribute.GetName());
    m_button = new QPushButton(QString::fromStdString(buttonText));
    connect(m_button, SIGNAL(clicked(bool)), this, SLOT(OnButtonClicked(bool)));
    layout->addWidget(m_button);

    AfterConstructor();
    m_layout->setAlignment(Qt::AlignRight);
}

void AttrWidgetButton::SetValue(const String &buttonText)
{
    m_button->setText(QString::fromStdString(buttonText));
}

String AttrWidgetButton::GetValue() const
{
    return m_button->text().toStdString();
}

void AttrWidgetButton::Refresh(const XMLAttribute &attribute)
{
    AttributeWidget::Refresh(attribute);
    if (attribute.GetType() != XMLAttribute::Type::Button) return;
}

void AttrWidgetButton::OnButtonClicked(bool _)
{
    if (m_listener)
    {
        m_listener->OnButtonClicked(GetValue());
    }
}

QSize AttrWidgetButton::sizeHint() const
{
    return QSize(100, 30);
}


