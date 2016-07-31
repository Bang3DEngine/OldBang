#include "AttrWidgetString.h"

AttrWidgetString::AttrWidgetString(const std::string &labelString,
                                   InspectorWidget *parent,
                                   bool readonly, bool inlined) :
    AttributeWidget(labelString, parent)
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
    setLayout(layout);

    m_strField = new LabelStringSW(); //Right side
    m_strField->setReadOnly(readonly);
    m_strField->setAlignment(Qt::AlignRight);
    m_strField->setMinimumWidth(50);
    m_strField->setMinimumHeight(25);
    m_strField->setContentsMargins(0,0,0,0);
    m_strField->show();

    layout->addWidget(GetLabelWidget(labelString));
    layout->addWidget(m_strField);

    setContentsMargins(0,0,0,0);
    show();

    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
}

void AttrWidgetString::SetValue(const std::string &value)
{
    if (!m_editing)
    {
        //disconnect(m_strField, SIGNAL(valueChanged(QString)), m_parent, SLOT(_OnSlotValueChanged(QString)));
        m_strField->setText( QString::fromStdString(value) );
        m_strField->show();
        //connect(m_strField, SIGNAL(valueChanged(QString)), m_parent, SLOT(_OnSlotValueChanged(QString)));
    }
}

const std::string AttrWidgetString::GetValue() const
{
    std::string str = m_strField->text().toStdString();
    return str;
}

void AttrWidgetString::OnFocusIn()
{
    m_editing = true;
}

void AttrWidgetString::OnFocusOut()
{
    m_editing = false;
    //m_parent->_OnSlotValueChanged();
}

QSize AttrWidgetString::sizeHint() const
{
    return QSize(100, 30);
}


