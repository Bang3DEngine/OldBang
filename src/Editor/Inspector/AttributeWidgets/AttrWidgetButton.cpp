#include "Bang/AttrWidgetButton.h"

#include <QVBoxLayout>
#include "Bang/WinUndef.h"

AttrWidgetButton::AttrWidgetButton(const String &buttonText,
                                   IAttrWidgetButtonListener *listener)
{
    QLayout *layout = new QVBoxLayout();
    m_horizontalLayout.addLayout(layout, 1);

    m_listener = listener;

    m_button = new QPushButton(buttonText.ToQString());
    m_button->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    QObject::connect(m_button, SIGNAL(pressed()),
                     this, SLOT(OnButtonClicked()));
    layout->addWidget(m_button);

    SetHeightSizeHint(50);

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

void AttrWidgetButton::Refresh()
{
    AttributeWidget::Refresh();
}

void AttrWidgetButton::OnButtonClicked()
{
    if (m_listener)
    {
        m_listener->OnButtonClicked(this);
    }
}

QSize AttrWidgetButton::sizeHint() const
{
    return QSize(100, GetHeightSizeHint());
}


