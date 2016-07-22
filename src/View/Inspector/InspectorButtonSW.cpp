#include "InspectorButtonSW.h"

InspectorButtonSW::InspectorButtonSW(const std::string &label,
                                     InspectorWidget *parent,
                                     std::function<void()> onClickFunction) :
    InspectorSW("", parent)
{
    QLayout *layout = new QVBoxLayout();
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    this->setLayout(layout);

    this->m_onClickFunction = onClickFunction;

    m_button = new QPushButton(QString::fromStdString(label));
    connect(m_button, SIGNAL(clicked(bool)),
            this, SLOT(OnButtonClicked(bool)));
    m_button->setContentsMargins(0,0,0,0);
    m_button->show();

    layout->addWidget(m_button);

    this->setContentsMargins(0,0,0,0);
    this->show();

    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
}

void InspectorButtonSW::OnButtonClicked(bool _)
{
    m_onClickFunction();
}

QSize InspectorButtonSW::sizeHint() const
{
    return QSize(100, 30);
}


