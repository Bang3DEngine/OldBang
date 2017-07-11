#ifndef INSPECTORSTRINGSW_H
#define INSPECTORSTRINGSW_H

#include <QTimer>
#include <QLineEdit>
#include <QTextEdit>
#include <QFocusEvent>
#include <QDoubleSpinBox>
#include "Bang/WinUndef.h"

#include "Bang/AttributeWidget.h"

class TextEdit;
class LineEdit;
class AttrWidgetString : public AttributeWidget
{
    Q_OBJECT

private:
    LineEdit *m_lineEdit = nullptr;
    TextEdit *m_textEdit = nullptr;
    bool m_editing = false;

public:
    AttrWidgetString(const XMLAttribute &xmlAttribute);

    virtual void SetValue(const String &value);
    virtual String GetValue() const;

    virtual void Refresh(const XMLAttribute &attribute) override;

    virtual void OnFocusIn();
    virtual void OnFocusOut();
    virtual void OnKeyPressed();
    QSize sizeHint() const override;
};


class LineEdit : public QLineEdit //Slot for a float (label + float)
{
    Q_OBJECT

public:
    LineEdit(bool readonly);

    virtual void focusInEvent(QFocusEvent * event) override;
    virtual void focusOutEvent(QFocusEvent * event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;

public slots:

    void Deselect();
    void SelectAll();
};

class TextEdit : public QTextEdit //Slot for a float (label + float)
{
    Q_OBJECT

public:
    TextEdit(bool readonly);

    QSize sizeHint() const override;

    virtual void focusInEvent(QFocusEvent * event) override;
    virtual void focusOutEvent(QFocusEvent * event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
};

#endif // INSPECTORSTRINGSW_H
