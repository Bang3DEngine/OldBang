#include "InspectorStringSW.h"

InspectorStringSW::InspectorStringSW(const std::string &labelString,
                                     const std::string &initialValue,
                                     InspectorWidget *parent,
                                     bool readonly,
                                     bool inlined) :
    InspectorSW(labelString, parent)
{

    QLayout *layout = nullptr;
    if(inlined)
    {
        layout = new QHBoxLayout();
    }
    else
    {
        layout = new QVBoxLayout();
    }
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    this->setLayout(layout);


    QLabel *labelField = new QLabel(QString::fromStdString(labelString));
    labelField->setAlignment(Qt::AlignLeft);
    labelField->setContentsMargins(0,0,0,0);
    labelField->show();

    strField = new LabelStringSW(); //Right side
    strField->setText( QString::fromStdString(initialValue) );
    strField->setReadOnly(readonly);
    strField->setAlignment(Qt::AlignRight);
    strField->setMinimumWidth(50);
    strField->setContentsMargins(0,0,0,0);
    strField->show();

    layout->addWidget(labelField);
    layout->addWidget(strField);

    this->setContentsMargins(0,0,0,0);
    this->show();

    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
}

void InspectorStringSW::SetValue(const std::string &value)
{
    if(!editing)
    {
        disconnect(strField, SIGNAL(valueChanged(QString)), parent, SLOT(_OnSlotValueChanged(QString)));
        strField->setText( QString::fromStdString(value) );
        strField->show();
        connect(strField, SIGNAL(valueChanged(QString)), parent, SLOT(_OnSlotValueChanged(QString)));
    }
}

const std::string InspectorStringSW::GetValue() const
{
    std::string str = strField->text().toStdString();
    return str;
}

void InspectorStringSW::OnFocusIn()
{
    editing = true;
}

void InspectorStringSW::OnFocusOut()
{
    editing = false;
    //parent->_OnSlotValueChanged();
}

QSize InspectorStringSW::sizeHint() const
{
    return QSize(100, 30);
}


