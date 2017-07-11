#include "Bang/AttrWidgetInt.h"

#include <QVBoxLayout>
#include "Bang/WinUndef.h"

#include "Bang/Object.h"
#include "Bang/XMLAttribute.h"
#include "Bang/InspectorWidget.h"

AttrWidgetInt::AttrWidgetInt(const XMLAttribute &xmlAttribute,
                             bool isSubWidget) :
    AttributeWidget(xmlAttribute, isSubWidget, !isSubWidget)
{
    QVBoxLayout *layout = new QVBoxLayout();
    m_horizontalLayout.addLayout(layout, 1);
    m_horizontalLayout.setMargin(1);

    m_lineEdit = new IntComponentSlotSpinBox();
    m_lineEdit->setAlignment(Qt::AlignLeft);
    m_lineEdit->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    QObject::connect(m_lineEdit, SIGNAL(textChanged(QString)),
                     this, SIGNAL(OnValueChanged()));

    layout->addWidget(m_lineEdit);

    setMinimumWidth(15);
    setContentsMargins(0, 0, 0, 0);

    AfterConstructor();
}

void AttrWidgetInt::SetValue(int a)
{
    if (!_editing)
    {
        blockSignals(true);
        m_lineEdit->SetInt(a);
        blockSignals(false);
    }
}

int AttrWidgetInt::GetValue() const
{
    return m_lineEdit->GetInt();
}

void AttrWidgetInt::Refresh(const XMLAttribute &attribute)
{
    AttributeWidget::Refresh(attribute);

    if (attribute.GetType() != XMLAttribute::Type::Int) return;
    SetValue( attribute.GetInt() );
}

XMLAttribute AttrWidgetInt::GetXMLAttribute() const
{
    XMLAttribute attr;
    attr.SetInt(GetValue());
    return attr;
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
    Object::SCast<AttrWidgetInt>(parent())->OnLineEditFocusIn();
}

void IntComponentSlotSpinBox::focusOutEvent(QFocusEvent *event)
{
    QLineEdit::focusOutEvent(event);
    Object::SCast<AttrWidgetInt>(parent())->OnLineEditFocusOut();
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

void IntComponentSlotSpinBox::SetInt(int a)
{
    String str = String(a);
    blockSignals(true);
    setText(str.ToQString());
    blockSignals(false);
}

int IntComponentSlotSpinBox::GetInt() const
{
    String str( text() );
    str = str.Replace(",", ".");
    if (str.Empty()) { return 0; }
    return String::ToInt(str);
}

void IntComponentSlotSpinBox::SelectAll()
{
    selectAll();
}

