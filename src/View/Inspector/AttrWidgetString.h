#ifndef INSPECTORSTRINGSW_H
#define INSPECTORSTRINGSW_H

#include <string>
#include <QTimer>
#include <QFocusEvent>
#include <QDoubleSpinBox>

#include "AttributeWidget.h"

class LabelStringSW;
class AttrWidgetString : public AttributeWidget
{
    Q_OBJECT

private:
    LabelStringSW *m_strField = nullptr;
    bool m_editing = false;

public:
    AttrWidgetString(const std::string &labelString,
                     InspectorWidget *m_parent,
                     bool readonly = false,
                     bool inlined = false);

    virtual void SetValue(const std::string &value);
    virtual const std::string GetValue() const;

    virtual void OnFocusIn();
    virtual void OnFocusOut();
    QSize sizeHint() const override;
};


class LabelStringSW : public QLineEdit //Slot for a float (label + float)
{
    Q_OBJECT

public:
    LabelStringSW() : QLineEdit()
    {
    }

    virtual void focusInEvent(QFocusEvent * event) override
    {
        QLineEdit::focusInEvent(event);
        QTimer::singleShot(50, this, SLOT(SelectAll()));
        static_cast<AttrWidgetString*>(parent())->OnFocusIn();
    }

    virtual void focusOutEvent(QFocusEvent * event) override
    {
        QLineEdit::focusOutEvent(event);
        static_cast<AttrWidgetString*>(parent())->OnFocusOut();
    }

    virtual void  keyPressEvent(QKeyEvent *event) override
    {
        QLineEdit::keyPressEvent(event);
        if (event->key() == QKeyEvent::Enter)
        {
            static_cast<AttrWidgetString*>(parent())->OnFocusOut();
        }
    }

public slots:

    void SelectAll()
    {
        if (!this->isReadOnly())
        {
            this->selectAll();
        }
    }
};

#endif // INSPECTORSTRINGSW_H
