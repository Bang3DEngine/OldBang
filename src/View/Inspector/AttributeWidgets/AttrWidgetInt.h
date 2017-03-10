#ifndef ATTRWIDGETINT_H
#define ATTRWIDGETINT_H


#include <QTimer>
#include <QLineEdit>
#include <QFocusEvent>
#include <QDoubleSpinBox>

#include "AttributeWidget.h"

class XMLAttribute;
class InspectorWidget;
class IntComponentSlotSpinBox;
class AttrWidgetInt : public AttributeWidget //Slot for a float (label + float)
{
    Q_OBJECT

private:
    IntComponentSlotSpinBox *m_lineEdit = nullptr;
    bool _editing = false;

public:

    AttrWidgetInt(const XMLAttribute &xmlAttribute,
                  InspectorWidget *m_inspectorWidget,
                  bool isSubWidget = false);

    virtual void SetValue(int a);
    virtual int GetValue();

    virtual void Refresh(const XMLAttribute &attribute) override;

    virtual void OnLineEditFocusIn();
    virtual void OnLineEditFocusOut();
};

class IntComponentSlotSpinBox : public QLineEdit //Slot for a float (label + float)
{
    Q_OBJECT

public:
    IntComponentSlotSpinBox();

    virtual void focusInEvent(QFocusEvent * event) override;
    virtual void focusOutEvent(QFocusEvent * event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;

    void SetInt(int a);
    int GetInt() const;

public slots:
    void SelectAll();
};

#endif // ATTRWIDGETINT_H
