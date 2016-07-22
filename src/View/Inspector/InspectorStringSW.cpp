#include "InspectorStringSW.h"

InspectorStringSW::InspectorStringSW(const std::string &labelString,
                                     const std::string &initialValue,
                                     InspectorWidget *parent,
                                     bool readonly,
                                     bool inlined) :
    InspectorSW(labelString, parent)
{

    QLayout *layout = nullptr;
    if (inlined)
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

    p_strField = new LabelStringSW(); //Right side
    p_strField->setText( QString::fromStdString(initialValue) );
    p_strField->setReadOnly(readonly);
    p_strField->setAlignment(Qt::AlignRight);
    p_strField->setMinimumWidth(50);
    p_strField->setContentsMargins(0,0,0,0);
    p_strField->show();

    layout->addWidget(labelField);
    layout->addWidget(p_strField);

    this->setContentsMargins(0,0,0,0);
    this->show();

    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
}

void InspectorStringSW::SetValue(const std::string &value)
{
    if (!m_editing)
    {
        disconnect(p_strField, SIGNAL(valueChanged(QString)), p_parent, SLOT(_OnSlotValueChanged(QString)));
        p_strField->setText( QString::fromStdString(value) );
        p_strField->show();
        connect(p_strField, SIGNAL(valueChanged(QString)), p_parent, SLOT(_OnSlotValueChanged(QString)));
    }
}

const std::string InspectorStringSW::GetValue() const
{
    std::string str = p_strField->text().toStdString();
    return str;
}

void InspectorStringSW::OnFocusIn()
{
    m_editing = true;
}

void InspectorStringSW::OnFocusOut()
{
    m_editing = false;
    //parent->_OnSlotValueChanged();
}

QSize InspectorStringSW::sizeHint() const
{
    return QSize(100, 30);
}


