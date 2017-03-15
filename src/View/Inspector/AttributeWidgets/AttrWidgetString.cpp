#include "AttrWidgetString.h"

#include <QVBoxLayout>

#include "Object.h"
#include "InspectorWidget.h"


AttrWidgetString::AttrWidgetString(const XMLAttribute &xmlAttribute,
                                   InspectorWidget *inspectorWidget) :
    AttributeWidget(xmlAttribute, inspectorWidget, false, true,
                    xmlAttribute.HasProperty(XMLProperty::BigText))
{
    QLayout *layout = new QVBoxLayout();
    m_layout.addLayout(layout, 0);
    m_layout.setContentsMargins(0,0,0,0);

    bool bigText = xmlAttribute.HasProperty(XMLProperty::BigText);
    QWidget *textWidget = nullptr;
    if (!bigText)
    {
        m_lineEdit = new LineEdit(m_readonly); //Right side
        m_lineEdit->setAlignment(Qt::AlignLeft);
        m_lineEdit->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
        textWidget = m_lineEdit;
    }
    else
    {
        m_textEdit = new TextEdit(m_readonly);
        m_textEdit->setAlignment(Qt::AlignLeft);
        m_textEdit->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
        m_heightSizeHint = 550;
        textWidget = m_textEdit;
    }

    //connect(m_strField, SIGNAL(textChanged()),
    //        m_parent, SLOT(_OnSlotValueChanged()));

    m_layout.addWidget(textWidget);

    AfterConstructor();
}

void AttrWidgetString::SetValue(const String &value)
{
    if (!m_editing)
    {
        if (m_lineEdit)
        {
            //disconnect(m_strField, SIGNAL(textChanged()), m_parent, SLOT(_OnSlotValueChanged()));
            m_lineEdit->setText( value.ToQString() );
            //connect(m_strField, SIGNAL(textChanged()), m_parent, SLOT(_OnSlotValueChanged()));
        }
        else if (m_textEdit)
        {
            m_textEdit->setText( value.ToQString() );
        }
    }
}

String AttrWidgetString::GetValue() const
{
    if (m_lineEdit)
    {
        return m_lineEdit->text().toStdString();
    }
    else if (m_textEdit)
    {
        return m_textEdit->toPlainText().toStdString();
    }
    return "";
}

void AttrWidgetString::Refresh(const XMLAttribute &attribute)
{
    AttributeWidget::Refresh(attribute);
    ASSERT (attribute.GetType() == XMLAttribute::Type::String);
    SetValue( attribute.GetString() );
}

void AttrWidgetString::OnFocusIn()
{
    m_editing = true;
}

void AttrWidgetString::OnFocusOut()
{
    m_editing = false;
    m_inspectorWidget->_OnSlotValueChanged();
}

void AttrWidgetString::OnKeyPressed()
{
    m_inspectorWidget->_OnSlotValueChanged();
}

QSize AttrWidgetString::sizeHint() const
{
    return AttributeWidget::sizeHint();
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
    Object::SCast<AttrWidgetString>(parent())->OnFocusIn();
}

void LineEdit::focusOutEvent(QFocusEvent *event)
{
    QLineEdit::focusOutEvent(event);
    Object::SCast<AttrWidgetString>(parent())->OnFocusOut();
}

void LineEdit::keyPressEvent(QKeyEvent *event)
{
    QLineEdit::keyPressEvent(event);
    Object::SCast<AttrWidgetString>(parent())->OnKeyPressed();
}

void LineEdit::Deselect()
{
    deselect();
}

void LineEdit::SelectAll()
{
    if (!isReadOnly())
    {
        selectAll();
    }
}

////////////////////////////////////////////////////////////

TextEdit::TextEdit(bool readonly)
{
    // setReadOnly(readonly);
    setReadOnly(true);
    setMinimumHeight(500);
}

QSize TextEdit::sizeHint() const
{
    QSize defaultSize = QTextEdit::sizeHint();
    return QSize(defaultSize.width(), 500);
}

void TextEdit::focusInEvent(QFocusEvent *event)
{
    QTextEdit::focusInEvent(event);
    Object::SCast<AttrWidgetString>(parent())->OnFocusIn();
}

void TextEdit::focusOutEvent(QFocusEvent *event)
{
    QTextEdit::focusOutEvent(event);
    Object::SCast<AttrWidgetString>(parent())->OnFocusOut();
}

void TextEdit::keyPressEvent(QKeyEvent *event)
{
    QTextEdit::keyPressEvent(event);
    Object::SCast<AttrWidgetString>(parent())->OnKeyPressed();
}
