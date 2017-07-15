#include "Bang/AttrWidgetString.h"

#include <QVBoxLayout>
#include "Bang/WinUndef.h"

#include "Bang/Object.h"
#include "Bang/InspectorWidget.h"


AttrWidgetString::AttrWidgetString(const String &labelText) :
    AttributeWidget()
{
    QLayout *layout = new QVBoxLayout();
    m_horizontalLayout.addLayout(layout, 0);
    m_horizontalLayout.setContentsMargins(0,0,0,0);

    bool bigText = false;
    bool readonly = false;
    QWidget *textWidget = nullptr;
    if (!bigText)
    {
        m_lineEdit = new LineEdit(readonly); //Right side
        m_lineEdit->setAlignment(Qt::AlignLeft);
        m_lineEdit->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
        textWidget = m_lineEdit;

        QObject::connect(m_lineEdit, SIGNAL(textChanged(QString)),
                         this, SLOT(OnValueChanged()));
    }
    else
    {
        m_textEdit = new TextEdit(readonly);
        m_textEdit->setAlignment(Qt::AlignLeft);
        m_textEdit->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
        SetHeightSizeHint(550);
        textWidget = m_textEdit;

        QObject::connect(m_textEdit, SIGNAL(textChanged(QString)),
                         this, SLOT(OnValueChanged()));
    }

    m_horizontalLayout.addWidget(textWidget);

    SetLabelText(labelText);
}

void AttrWidgetString::SetValue(const String &value)
{
    if (!m_editing)
    {
        if (m_lineEdit)
        {
            m_lineEdit->blockSignals(true);
            m_lineEdit->setText( value.ToQString() );
            m_lineEdit->blockSignals(false);
        }
        else if (m_textEdit)
        {
            m_textEdit->blockSignals(true);
            m_textEdit->setText( value.ToQString() );
            m_textEdit->blockSignals(false);
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

void AttrWidgetString::OnUpdate()
{
    AttributeWidget::OnUpdate();
}

void AttrWidgetString::OnFocusIn()
{
    m_editing = true;
}

void AttrWidgetString::OnFocusOut()
{
    m_editing = false;
    emit OnValueChanged(this);
}

void AttrWidgetString::OnKeyPressed()
{
    emit OnValueChanged(this);
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
