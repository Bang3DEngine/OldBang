#include "AttrWidgetButton.h"

AttrWidgetButton::AttrWidgetButton(const std::string &label,
                                   std::function<void()> onClickFunction,
                                   InspectorWidget *parent) :
    AttributeWidget("", parent)
{
    QLayout *layout = new QVBoxLayout();
    setLayout(layout);

    this->m_onClickFunction = onClickFunction;

    m_button = new QPushButton(QString::fromStdString(label));
    connect(m_button, SIGNAL(clicked(bool)), this, SLOT(OnButtonClicked(bool)));
    m_button->show();

    layout->addWidget(m_button);

    show();

    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
}

void AttrWidgetButton::OnButtonClicked(bool _)
{
    m_onClickFunction();
}

QSize AttrWidgetButton::sizeHint() const
{
    return QSize(100, 30);
}


