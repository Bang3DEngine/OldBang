#ifndef ATTRWIDGETBOOL_H
#define ATTRWIDGETBOOL_H


#include <string>
#include <QTimer>
#include <QFocusEvent>
#include <QDoubleSpinBox>

#include "AttributeWidget.h"

class CheckboxWidget;
class AttrWidgetBool : public AttributeWidget
{
    Q_OBJECT

private:
    CheckboxWidget *m_checkbox = nullptr;

public:
    AttrWidgetBool(const std::string &labelString,
                   InspectorWidget *parent);

    virtual void SetValue(bool value);
    virtual bool GetValue() const;
};

class CheckboxWidget : public QCheckBox
{
    Q_OBJECT

public:
    CheckboxWidget() : QCheckBox()
    {
    }

    /*
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
    */
};



#endif // ATTRWIDGETBOOL_H
