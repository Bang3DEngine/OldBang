#ifndef INSPECTORFLOATSLOTWIDGET_H
#define INSPECTORFLOATSLOTWIDGET_H

#include <QTimer>
#include <QLineEdit>
#include <QFocusEvent>
#include <QDoubleSpinBox>
#include "Bang/WinUndef.h"

#include "Bang/AttributeWidget.h"

class InspectorWidget;
class FloatComponentSlotSpinBox;
class AttrWidgetFloat : public AttributeWidget
{
    Q_OBJECT

public:

    AttrWidgetFloat(const String &labelText);

    virtual void SetValue(float f);
    virtual float GetValue() const;

    virtual void OnUpdate() override;

    virtual void OnLineEditFocusIn();
    virtual void OnLineEditFocusOut();

private:
    bool m_editing = false;
    FloatComponentSlotSpinBox *m_lineEdit = nullptr;

private slots:
    void OnValueChanged(const QString &text);
};

class FloatComponentSlotSpinBox : public QLineEdit
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
