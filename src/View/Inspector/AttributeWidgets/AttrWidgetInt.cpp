#include "AttrWidgetInt.h"

#include <QVBoxLayout>

#include "StringUtils.h"
#include "XMLAttribute.h"
#include "InspectorWidget.h"

AttrWidgetInt::AttrWidgetInt(const XMLAttribute &xmlAttribute,
                             InspectorWidget *inspectorWidget,
                             bool isSubWidget) :
    AttributeWidget(xmlAttribute, inspectorWidget, isSubWidget, !isSubWidget)
{
    QVBoxLayout *layout = new QVBoxLayout();
    m_layout->addLayout(layout, 1);
    m_layout->setMargin(1);

    m_lineEdit = new IntComponentSlotSpinBox();
    m_lineEdit->setAlignment(Qt::AlignLeft);

    layout->addWidget(m_lineEdit);

    setMinimumWidth(15);
    setContentsMargins(0, 0, 0, 0);

    AfterConstructor();
}

void AttrWidgetInt::SetValue(int a)
{
    if (!_editing)
    {
        disconnect(m_lineEdit, SIGNAL(textChanged(QString)),
                   m_inspectorWidget, SLOT(_OnSlotValueChanged(QString)));
        m_lineEdit->SetInt(a);
        connect(m_lineEdit, SIGNAL(textChanged(QString)),
                m_inspectorWidget, SLOT(_OnSlotValueChanged(QString)));
    }
}

int AttrWidgetInt::GetValue()
{
    return m_lineEdit->GetInt();
}

void AttrWidgetInt::Refresh(const XMLAttribute &attribute)
{
    AttributeWidget::Refresh(attribute);

    if (attribute.GetType() != XMLAttribute::Type::Int) return;
    SetValue( attribute.GetInt() );
}

void AttrWidgetInt::OnLineEditFocusIn()
{
    _editing = true;
}

void AttrWidgetInt::OnLineEditFocusOut()
{
    _editing = false;
}




IntComponentSlotSpinBox::IntComponentSlotSpinBox() : QLineEdit()
{
    setMinimumWidth(15);
    setFixedHeight(18);
    installEventFilter(this);
}

void IntComponentSlotSpinBox::focusInEvent(QFocusEvent *event)
{
    QLineEdit::focusInEvent(event);
    QTimer::singleShot(50, this, SLOT(SelectAll()));
    static_cast<AttrWidgetInt*>(parent())->OnLineEditFocusIn();
}

void IntComponentSlotSpinBox::focusOutEvent(QFocusEvent *event)
{
    QLineEdit::focusOutEvent(event);
    static_cast<AttrWidgetInt*>(parent())->OnLineEditFocusOut();
}

void IntComponentSlotSpinBox::keyPressEvent(QKeyEvent *event)
{
    QLineEdit::keyPressEvent(event);

    if (event->key() == Qt::Key::Key_Enter)
    {
        clearFocus();
    }
    else if (event->key() == Qt::Key::Key_Up)
    {
    }
    else if (event->key() == Qt::Key::Key_Down)
    {
    }
}

bool IntComponentSlotSpinBox::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::Wheel && obj == this)
    {
        return true; // Block wheel event :D
    }

    return false;
}

void IntComponentSlotSpinBox::SetInt(int a)
{
    String str = String(a);
    setText(str.ToQString());
}

int IntComponentSlotSpinBox::GetInt() const
{
    String str( text() );
    StringUtils::Replace(&str, ",", ".");
    if (str.Empty()) { return 0; }
    return StringUtils::ToInt(str);
}

void IntComponentSlotSpinBox::SelectAll()
{
    selectAll();
}

