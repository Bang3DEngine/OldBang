#ifndef INSPECTORFLOATPARTSLOTWIDGET_H
#define INSPECTORFLOATPARTSLOTWIDGET_H

#include <string>
#include <QTimer>
#include <QFocusEvent>
#include <QDoubleSpinBox>

#include "InspectorPartSlotWidget.h"

class FloatPartSlotSpinBox;
class InspectorFloatPartSlotWidget : public InspectorPartSlotWidget //Slot for a float (label + float)
{
    Q_OBJECT

private:
    FloatPartSlotSpinBox *spinbox;

    bool editing;

public:
    InspectorFloatPartSlotWidget(float initialValue, const std::string &labelString,
                                 InspectorPartWidget *parent);

    virtual void SetValue(float f);
    virtual float GetValue();

    virtual void OnSpinBoxFocusIn();
    virtual void OnSpinBoxFocusOut();
    QSize sizeHint() const override;
};

class FloatPartSlotSpinBox : public QDoubleSpinBox //Slot for a float (label + float)
{
    Q_OBJECT

public:
    FloatPartSlotSpinBox() : QDoubleSpinBox()
    {
        connect(this, SIGNAL(valueChanged(double)), this, SLOT(AdjustStep(double)));
    }

    virtual void focusInEvent(QFocusEvent * event) override
    {
        QDoubleSpinBox::focusInEvent(event);
        QTimer::singleShot(50, this, SLOT(SelectAll()));
        static_cast<InspectorFloatPartSlotWidget*>(parent())->OnSpinBoxFocusIn();
    }

    virtual void focusOutEvent(QFocusEvent * event) override
    {
        QDoubleSpinBox::focusOutEvent(event);
        static_cast<InspectorFloatPartSlotWidget*>(parent())->OnSpinBoxFocusOut();
    }

    virtual void  keyPressEvent(QKeyEvent *event) override
    {
        QDoubleSpinBox::keyPressEvent(event);
        if(event->key() == QKeyEvent::Enter)
        {
            static_cast<InspectorFloatPartSlotWidget*>(parent())->OnSpinBoxFocusOut();
        }
    }

public slots:
    void AdjustStep(double v)
    {
        if(abs(v) <= 0.1f) v = 0.1f;
        setSingleStep( pow(10.0, int(log10(v == 0.0f ? v + 0.1f : v))-1) );
    }

    void SelectAll() { selectAll(); }
};

#endif // INSPECTORFLOATPARTSLOTWIDGET_H
