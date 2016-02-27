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
#include "ListInspectorItemInfo.h"
#include "IWindowEventManagerListener.h"

class Part;
class WindowEventManager;
class ListInspectorPartWidget : public QWidget
{
    //Every part widget, has many slots,
    // representing all its attributes (vectors, strings, integers, etc.)
    Q_OBJECT

private:
    class WidgetSlot;

    Part *relatedPart; //Set by ListInspector when creating it
    QLabel *titleLabel;
    std::vector<WidgetSlot*> partSlots;
    std::map<std::string, WidgetSlot*> labelsToPartSlots;

    QTimer *updateTimer;


    // ////////////////////////
    static const std::string FloatToString(float f);
    class WidgetSlot : public QWidget
    {
        protected:
            WidgetSlot() : QWidget(nullptr) {}
    };

    class WidgetSlotFloat : public WidgetSlot //Slot for a float (label + float)
    {
        public:
            QDoubleSpinBox *spinbox;

            WidgetSlotFloat(float initialValue, const std::string &labelString,ListInspectorPartWidget *parent);
            virtual void SetValue(float f);
            virtual float GetValue();

            QSize sizeHint() const override { return QSize(50, 50); }
    };

    class WidgetSlotVectorFloat : public WidgetSlot //Slot for a vector of size N
    {
        public:
            std::vector<WidgetSlotFloat*> floatSlots;

            WidgetSlotVectorFloat(std::vector<float> initialValues, const std::string &labelString,
                                  ListInspectorPartWidget *parent);

            virtual void SetValue(const std::vector<float> &v);
            virtual std::vector<float> GetValue();
    };
    // /////////////////////////

public:
    explicit ListInspectorPartWidget(Part *relatedPart);
    virtual ~ListInspectorPartWidget();
    std::vector<float> GetSlotValueVecFloat(const std::string &slotLabel);

public slots:
    void UpdateSlotsValues();
    void _NotifyInspectorSlotChanged(double newValue);
};

#endif // LISTINSPECTORITEM_H
