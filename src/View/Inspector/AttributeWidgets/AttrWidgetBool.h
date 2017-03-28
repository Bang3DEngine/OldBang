#ifndef ATTRWIDGETBOOL_H
#define ATTRWIDGETBOOL_H

#include <QTimer>
#include <QCheckBox>
#include <QFocusEvent>

#include "Bang/XMLAttribute.h"
#include "Bang/InspectorWidget.h"
#include "Bang/AttributeWidget.h"

class CheckboxWidget;
class AttrWidgetBool : public AttributeWidget
{
    Q_OBJECT

private:
    CheckboxWidget *m_checkbox = nullptr;

public:
    AttrWidgetBool(const XMLAttribute &xmlAttribute,
                   InspectorWidget *inspectorWidget);

    virtual void SetValue(bool value);
    virtual bool GetValue() const;

    virtual void Refresh(const XMLAttribute &attribute) override;
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
        Object::SCast<AttrWidgetString>(parent())->OnFocusIn();
    }

    virtual void focusOutEvent(QFocusEvent * event) override
    {
        QLineEdit::focusOutEvent(event);
        Object::SCast<AttrWidgetString>(parent())->OnFocusOut();
    }

    virtual void  keyPressEvent(QKeyEvent *event) override
    {
        QLineEdit::keyPressEvent(event);
        if (event->key() == QKeyEvent::Enter)
        {
            Object::SCast<AttrWidgetString>(parent())->OnFocusOut();
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
