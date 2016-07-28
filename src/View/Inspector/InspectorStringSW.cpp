#include "InspectorStringSW.h"

InspectorStringSW::InspectorStringSW(const std::string &labelString,
                                     const std::string &initialValue,
                                     InspectorWidget *parent,
                                     XMLNode *relatedXMLInfo,
                                     bool readonly,
                                     bool inlined) :
    InspectorSW(labelString, parent, relatedXMLInfo)
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

    m_strField = new LabelStringSW(); //Right side
    m_strField->setText( QString::fromStdString(initialValue) );
    m_strField->setReadOnly(readonly);
    m_strField->setAlignment(Qt::AlignRight);
    m_strField->setMinimumWidth(50);
    m_strField->setContentsMargins(0,0,0,0);
    m_strField->show();

    layout->addWidget(labelField);
    layout->addWidget(m_strField);

    this->setContentsMargins(0,0,0,0);
    this->show();

    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
}

void InspectorStringSW::SetValue(const std::string &value)
{
    if (!m_editing)
    {
        disconnect(m_strField, SIGNAL(valueChanged(QString)), m_parent, SLOT(_OnSlotValueChanged(QString)));
        m_strField->setText( QString::fromStdString(value) );
        m_strField->show();
        connect(m_strField, SIGNAL(valueChanged(QString)), m_parent, SLOT(_OnSlotValueChanged(QString)));
    }
}

const std::string InspectorStringSW::GetValue() const
{
    std::string str = m_strField->text().toStdString();
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


