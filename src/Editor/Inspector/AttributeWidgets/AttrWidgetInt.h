#ifndef ATTRWIDGETINT_H
#define ATTRWIDGETINT_H


#include <QTimer>
#include <QLineEdit>
#include <QFocusEvent>
#include <QDoubleSpinBox>
#include "Bang/WinUndef.h"

#include "Bang/AttributeWidget.h"

class InspectorWidget;
class IntComponentSlotSpinBox;
class AttrWidgetInt : public AttributeWidget //Slot for a float (label + float)
{
    Q_OBJECT

private:
    IntComponentSlotSpinBox *m_lineEdit = nullptr;
    bool m_editing = false;

public:

    AttrWidgetInt(const String &labelText);

    virtual void SetValue(int a);
    virtual int GetValue() const;

    virtual void OnUpdate() override;

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
