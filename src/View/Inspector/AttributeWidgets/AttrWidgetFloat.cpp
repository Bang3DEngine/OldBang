#include "AttrWidgetFloat.h"

#include <QVBoxLayout>

#include "Object.h"
#include "XMLAttribute.h"
#include "InspectorWidget.h"

AttrWidgetFloat::AttrWidgetFloat(const XMLAttribute &xmlAttribute,
                                 InspectorWidget *inspectorWidget,
                                 bool isSubWidget) :
    AttributeWidget(xmlAttribute, inspectorWidget, isSubWidget, !isSubWidget)
{
    QVBoxLayout *layout = new QVBoxLayout();
    m_layout.addLayout(layout, 1);
    m_layout.setMargin(1);

    m_lineEdit = new FloatComponentSlotSpinBox();
    m_lineEdit->setAlignment(Qt::AlignLeft);
    m_lineEdit->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    // m_spinbox->setMaximum( 9999999999999999.9f);
    // m_spinbox->setMinimum(-9999999999999999.9f);

    layout->addWidget(m_lineEdit);

    setMinimumWidth(15);
    setContentsMargins(0, 0, 0, 0);

    AfterConstructor();
}

void AttrWidgetFloat::SetValue(float f)
{
    if (!_editing)
    {
        disconnect(m_lineEdit, SIGNAL(textChanged(QString)),
                   m_inspectorWidget, SLOT(_OnSlotValueChanged(QString)));
        m_lineEdit->SetFloat(f);
        connect(m_lineEdit, SIGNAL(textChanged(QString)),
                m_inspectorWidget, SLOT(_OnSlotValueChanged(QString)));
    }
}

float AttrWidgetFloat::GetValue()
{
    return m_lineEdit->GetFloat();
}

void AttrWidgetFloat::Refresh(const XMLAttribute &attribute)
{
    AttributeWidget::Refresh(attribute);

    if (attribute.GetType() != XMLAttribute::Type::Float) return;
    SetValue( attribute.GetFloat() );
}

void AttrWidgetFloat::OnLineEditFocusIn()
{
    _editing = true;
}

void AttrWidgetFloat::OnLineEditFocusOut()
{
    _editing = false;
}




FloatComponentSlotSpinBox::FloatComponentSlotSpinBox() : QLineEdit()
{
    //connect(this, SIGNAL(valueChanged(double)), this, SLOT(AdjustStep(double)));
    setMinimumWidth(15);
    setFixedHeight(18);
    //AdjustStep();
    installEventFilter(this);
}

void FloatComponentSlotSpinBox::focusInEvent(QFocusEvent *event)
{
    QLineEdit::focusInEvent(event);
    //AdjustStep();
    QTimer::singleShot(50, this, SLOT(SelectAll()));
    Object::SCast<AttrWidgetFloat>(parent())->OnLineEditFocusIn();
}

void FloatComponentSlotSpinBox::focusOutEvent(QFocusEvent *event)
{
    QLineEdit::focusOutEvent(event);
    //AdjustStep();
    Object::SCast<AttrWidgetFloat>(parent())->OnLineEditFocusOut();
}

void FloatComponentSlotSpinBox::keyPressEvent(QKeyEvent *event)
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

void FloatComponentSlotSpinBox::SetFloat(float f)
{
    String str = String::ToString(f, 2);
    setText( str.ToQString() );
}

float FloatComponentSlotSpinBox::GetFloat() const
{
    String str( text() );
    str = str.Replace(",", ".");
    if (str.Empty()) { return 0.0f; }
    return String::ToFloat(str);
}

void FloatComponentSlotSpinBox::AdjustStep()
{
    //if (std::abs(v) <= 1.0f) setSingleStep(0.1f);
    //else setSingleStep( pow(10.0, int(log10(v == 0.0f ? 0.1f : std::abs(v)))-1) );
}

void FloatComponentSlotSpinBox::SelectAll()
{
    selectAll();
}
