#include "Bang/AttrWidgetButton.h"

#include <QVBoxLayout>
#include "Bang/WinUndef.h"

#include "Bang/Inspector.h"

AttrWidgetButton::AttrWidgetButton(const XMLAttribute &xmlAttribute) :
    AttributeWidget(xmlAttribute, false, false)
{
    QLayout *layout = new QVBoxLayout();
    m_horizontalLayout.addLayout(layout, 1);

    m_listener = xmlAttribute.GetButtonListener();

    String buttonText = Inspector::FormatInspectorLabel(xmlAttribute.GetName());
    m_button = new QPushButton(buttonText.ToQString());
    m_button->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    QObject::connect(m_button, SIGNAL(clicked(bool)),
                     this, SLOT(OnButtonClicked(bool)));
    layout->addWidget(m_button);

    SetHeightSizeHint(50);

    AfterConstructor();
    m_horizontalLayout.setAlignment(Qt::AlignRight);
}

void AttrWidgetButton::SetValue(const String &buttonText)
{
    m_button->blockSignals(true);
    m_button->setText(buttonText.ToQString());
    m_button->blockSignals(false);
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

XMLAttribute AttrWidgetButton::GetXMLAttribute() const
{
    XMLAttribute attr;
    attr.SetButton(GetValue(), m_listener );
    return attr;
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
    return QSize(100, GetHeightSizeHint());
}


