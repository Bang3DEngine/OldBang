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

    p_button = new QPushButton(QString::fromStdString(label));
    connect(p_button, SIGNAL(clicked(bool)),
            this, SLOT(OnButtonClicked(bool)));
    p_button->setContentsMargins(0,0,0,0);
    p_button->show();

    layout->addWidget(p_button);

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


