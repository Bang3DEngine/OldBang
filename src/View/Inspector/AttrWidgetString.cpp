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

    m_strField = new LineEdit(readonly); //Right side
    m_strField->setAlignment(Qt::AlignRight);
    m_strField->setMinimumWidth(50);
    m_strField->setMinimumHeight(25);
    m_strField->setContentsMargins(0,0,0,0);
    m_strField->show();

    //connect(m_strField, SIGNAL(textChanged()), m_parent, SLOT(_OnSlotValueChanged()));

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
        //disconnect(m_strField, SIGNAL(textChanged()), m_parent, SLOT(_OnSlotValueChanged()));
        m_strField->setText( QString::fromStdString(value) );
        m_strField->show();
        //connect(m_strField, SIGNAL(textChanged()), m_parent, SLOT(_OnSlotValueChanged()));
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

////////////////////////////////////////////////////////////

LineEdit::LineEdit(bool readonly) : QLineEdit()
{
    setReadOnly(readonly);
    if (readonly)
    {
        connect (this, SIGNAL(selectionChanged()),
                 this, SLOT(Deselect()));
    }
}

void LineEdit::focusInEvent(QFocusEvent *event)
{
    QLineEdit::focusInEvent(event);
    QTimer::singleShot(50, this, SLOT(SelectAll()));
    static_cast<AttrWidgetString*>(parent())->OnFocusIn();
}

void LineEdit::focusOutEvent(QFocusEvent *event)
{
    QLineEdit::focusOutEvent(event);
    static_cast<AttrWidgetString*>(parent())->OnFocusOut();
}

void LineEdit::keyPressEvent(QKeyEvent *event)
{
    QLineEdit::keyPressEvent(event);
    if (event->key() == QKeyEvent::Enter)
    {
        static_cast<AttrWidgetString*>(parent())->OnFocusOut();
    }
}

void LineEdit::Deselect()
{
    deselect();
}

void LineEdit::SelectAll()
{
    if (!this->isReadOnly())
    {
        this->selectAll();
    }
}


