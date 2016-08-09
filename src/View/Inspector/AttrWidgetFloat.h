#ifndef INSPECTORFLOATSLOTWIDGET_H
#define INSPECTORFLOATSLOTWIDGET_H

#include <string>
#include <QTimer>
#include <QFocusEvent>
#include <QDoubleSpinBox>

#include "AttributeWidget.h"

class FloatComponentSlotSpinBox;
class AttrWidgetFloat : public AttributeWidget //Slot for a float (label + float)
{
    Q_OBJECT

private:
    FloatComponentSlotSpinBox *m_spinbox = nullptr;
    bool _editing = false;

public:
    AttrWidgetFloat(const XMLAttribute &xmlAttribute,
                    InspectorWidget *m_inspectorWidget);

    virtual void SetValue(float f);
    virtual float GetValue();

    virtual void Refresh(const XMLAttribute &attribute) override;

    virtual void OnSpinBoxFocusIn();
    virtual void OnSpinBoxFocusOut();
};

class FloatComponentSlotSpinBox : public QDoubleSpinBox //Slot for a float (label + float)
{
    Q_OBJECT

public:
    FloatComponentSlotSpinBox() : QDoubleSpinBox()
    {
        //connect(this, SIGNAL(valueChanged(double)), this, SLOT(AdjustStep(double)));
        //setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        AdjustStep(value());
        installEventFilter(this);
        updateGeometry();
        show();
    }

    virtual void focusInEvent(QFocusEvent * event) override
    {
        QDoubleSpinBox::focusInEvent(event);
        AdjustStep(value());
        QTimer::singleShot(50, this, SLOT(SelectAll()));
        static_cast<AttrWidgetFloat*>(parent())->OnSpinBoxFocusIn();
    }

    virtual void focusOutEvent(QFocusEvent * event) override
    {
        QDoubleSpinBox::focusOutEvent(event);
        AdjustStep(value());
        static_cast<AttrWidgetFloat*>(parent())->OnSpinBoxFocusOut();
    }

    virtual void  keyPressEvent(QKeyEvent *event) override
    {
        QDoubleSpinBox::keyPressEvent(event);
    }

    bool eventFilter(QObject *obj, QEvent *event)
    {
        if (event->type() == QEvent::Wheel && obj == this)
        {
            return true; // Block wheel event :D
        }

        return false;
    }

public slots:
    void AdjustStep(double v)
    {
        if (std::abs(v) <= 1.0f) setSingleStep(0.1f);
        else setSingleStep( pow(10.0, int(log10(v == 0.0f ? 0.1f : std::abs(v)))-1) );
    }

    void SelectAll()
    {
        selectAll();
    }
};

#endif // INSPECTORFLOATSLOTWIDGET_H
