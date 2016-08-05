#include "AttrWidgetButton.h"

AttrWidgetButton::AttrWidgetButton(const std::string &label,
                                   IAttrWidgetButtonListener *listener,
                                   InspectorWidget *parent) :
    AttributeWidget("", parent)
{
    QLayout *layout = new QVBoxLayout();
    setLayout(layout);

    m_listener = listener;

    m_button = new QPushButton(QString::fromStdString(label));
    connect(m_button, SIGNAL(clicked(bool)), this, SLOT(OnButtonClicked(bool)));
    m_button->show();

    layout->addWidget(m_button);

    show();

    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
}

void AttrWidgetButton::SetValue(const std::string &buttonText)
{
    m_button->setText(QString::fromStdString(buttonText));
}

std::string AttrWidgetButton::GetValue() const
{
    return m_button->text().toStdString();
}

void AttrWidgetButton::OnButtonClicked(bool _)
{
    if (m_listener)
    {
        m_listener->OnButtonClicked();
    }
}

QSize AttrWidgetButton::sizeHint() const
{
    return QSize(100, 30);
}


