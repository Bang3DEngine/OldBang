#ifndef LISTINSPECTORITEMWIDGET_H
#define LISTINSPECTORITEMWIDGET_H

#include "Bang.h"

#include <string>
#include <iomanip>
#include <sstream>

#include <QLabel>
#include <QLayout>
#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QListWidget>
#include <QDoubleSpinBox>

#include "Logger.h"
#include "InspectorItemInfo.h"
#include "IWindowEventManagerListener.h"

class Part;
class WindowEventManager;
class InspectorPartWidget : public QWidget
{
    //Every part widget, has many slots,
    // representing all its attributes (vectors, strings, integers, etc.)
    Q_OBJECT

private:
    class SlotWidget;

    Part *relatedPart; //Set by Inspector when creating it
    QLabel *titleLabel;
    std::vector<SlotWidget*> partSlots;
    std::map<std::string, SlotWidget*> labelsToPartSlots;

    QTimer *updateTimer;


    // ////////////////////////
    static const std::string FloatToString(float f);
    class SlotWidget : public QWidget
    {
        protected:
        InspectorPartWidget *parent;

        SlotWidget(InspectorPartWidget *parent) : QWidget(nullptr), parent(parent) {}
        void focusInEvent(QFocusEvent * event) override;
        void focusOutEvent(QFocusEvent * event) override;
    };

    class FloatSlotWidget : public SlotWidget //Slot for a float (label + float)
    {
        public:
            QDoubleSpinBox *spinbox;

            FloatSlotWidget(float initialValue, const std::string &labelString,InspectorPartWidget *parent);
            virtual void SetValue(float f);
            virtual float GetValue();

            QSize sizeHint() const override { return QSize(50, 50); }
    };

    class VectorFloatSlotWidget : public SlotWidget //Slot for a vector of size N
    {
        public:
            std::vector<FloatSlotWidget*> floatSlots;

            VectorFloatSlotWidget(std::vector<float> initialValues, const std::string &labelString,
                                  InspectorPartWidget *parent);

            virtual void SetValue(const std::vector<float> &v);
            virtual std::vector<float> GetValue();
    };
    // /////////////////////////

public:
    explicit InspectorPartWidget(Part *relatedPart);
    virtual ~InspectorPartWidget();
    std::vector<float> GetVectorFloatSlotValue(const std::string &slotLabel);

public slots:
    void UpdateSlotsValues();
    void _NotifyInspectorSlotChanged(double newValue);
};

#endif // LISTINSPECTORITEM_H
