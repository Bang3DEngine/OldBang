#ifndef INSPECTORFLOATSLOTWIDGET_H
#define INSPECTORFLOATSLOTWIDGET_H

#include <QTimer>
#include <QLineEdit>
#include <QFocusEvent>
#include <QDoubleSpinBox>
#include "Bang/WinUndef.h"

#include "Bang/AttributeWidget.h"

class XMLAttribute;
class InspectorWidget;
class FloatComponentSlotSpinBox;
//Slot for a float (label + float)
class AttrWidgetFloat : public AttributeWidget
{
    Q_OBJECT

public:

    // isSubWidget indicates if it belongs to another parent attributeWidget,
    // such as AttrWidgetFloat's inside AttrWidgetVectorFloat
    AttrWidgetFloat(const XMLAttribute &xmlAttribute,
                    bool isSubWidget = false);

    virtual void SetValue(float f);
    virtual float GetValue() const;

    virtual void Refresh(const XMLAttribute &attribute) override;
    virtual XMLAttribute GetXMLAttribute() const override;

    virtual void OnLineEditFocusIn();
    virtual void OnLineEditFocusOut();

private:
    bool m_editing = false;
    FloatComponentSlotSpinBox *m_lineEdit = nullptr;
};

class FloatComponentSlotSpinBox : public QLineEdit //Slot for a float (label + float)
{
    Q_OBJECT

public:
    FloatComponentSlotSpinBox();

    virtual void focusInEvent(QFocusEvent * event) override;
    virtual void focusOutEvent(QFocusEvent * event) override;
    virtual void  keyPressEvent(QKeyEvent *event) override;

    void SetFloat(float f);
    float GetFloat() const;

public slots:
    void AdjustStep();
    void SelectAll();
};

#endif // INSPECTORFLOATSLOTWIDGET_H
