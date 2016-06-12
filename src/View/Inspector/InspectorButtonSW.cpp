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

    this->onClickFunction = onClickFunction;

    button = new QPushButton(QString::fromStdString(label));
    connect(button, SIGNAL(clicked(bool)),
            this, SLOT(OnButtonClicked(bool)));
    button->setContentsMargins(0,0,0,0);
    button->show();

    layout->addWidget(button);

    this->setContentsMargins(0,0,0,0);
    this->show();

    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
}

void InspectorButtonSW::OnButtonClicked(bool _)
{
    onClickFunction();
}

QSize InspectorButtonSW::sizeHint() const
{
    return QSize(100, 30);
}


