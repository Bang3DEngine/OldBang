#ifndef INSPECTORFLOATSLOTWIDGET_H
#define INSPECTORFLOATSLOTWIDGET_H


#include <QTimer>
#include <QFocusEvent>
#include <QDoubleSpinBox>

#include "AttributeWidget.h"

class FloatComponentSlotSpinBox;
class AttrWidgetFloat : public AttributeWidget //Slot for a float (label + float)
{
    Q_OBJECT

private:
    FloatComponentSlotSpinBox *m_lineEdit = nullptr;
    bool _editing = false;

public:

    // isSubWidget indicates if it belongs to another parent attributeWidget,
    // such as AttrWidgetFloat's inside AttrWidgetVectorFloat
    AttrWidgetFloat(const XMLAttribute &xmlAttribute,
                    InspectorWidget *m_inspectorWidget,
                    bool isSubWidget = false);

    virtual void SetValue(float f);
    virtual float GetValue();

    virtual void Refresh(const XMLAttribute &attribute) override;

    virtual void OnLineEditFocusIn();
    virtual void OnLineEditFocusOut();
};

class FloatComponentSlotSpinBox : public QLineEdit //Slot for a float (label + float)
{
    Q_OBJECT

public:
    FloatComponentSlotSpinBox() : QLineEdit()
    {
        //connect(this, SIGNAL(valueChanged(double)), this, SLOT(AdjustStep(double)));
        setMinimumWidth(15);
        setFixedHeight(18);
        //AdjustStep();
        installEventFilter(this);
    }

    virtual void focusInEvent(QFocusEvent * event) override
    {
        QLineEdit::focusInEvent(event);
        //AdjustStep();
        QTimer::singleShot(50, this, SLOT(SelectAll()));
        static_cast<AttrWidgetFloat*>(parent())->OnLineEditFocusIn();
    }

    virtual void focusOutEvent(QFocusEvent * event) override
    {
        QLineEdit::focusOutEvent(event);
        //AdjustStep();
        static_cast<AttrWidgetFloat*>(parent())->OnLineEditFocusOut();
    }

    virtual void  keyPressEvent(QKeyEvent *event) override
    {
        QLineEdit::keyPressEvent(event);

        if (event->key() == Qt::Key::Key_Enter)
        {
            clearFocus();
        }
        else if (event->key() == Qt::Key::Key_Up)
        {
        }
        else if (event->key() == Qt::Key::Key_Down)
        {
        }
    }

    bool eventFilter(QObject *obj, QEvent *event)
    {
        if (event->type() == QEvent::Wheel && obj == this)
        {
            return true; // Block wheel event :D
        }

        return false;
    }

    void SetFloat(float f)
    {
        String str = StringUtils::FromFloat(f, 2);
        setText(QString::fromStdString(str));
    }

    float GetFloat() const
    {
        String str = text().toStdString();
        StringUtils::Replace(&str, ",", ".");
        return StringUtils::ToFloat(str);
    }

public slots:
    void AdjustStep()
    {
        //if (std::abs(v) <= 1.0f) setSingleStep(0.1f);
        //else setSingleStep( pow(10.0, int(log10(v == 0.0f ? 0.1f : std::abs(v)))-1) );
    }

    void SelectAll()
    {
        selectAll();
    }
};

#endif // INSPECTORFLOATSLOTWIDGET_H
