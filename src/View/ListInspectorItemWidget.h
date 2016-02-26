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
class ListInspectorItemWidget : public QWidget
{
    Q_OBJECT

private:

    //Private function of the different Widget Slots
    static const std::string FloatToString(float f);

    //Slot for a float (label + float)
    class WidgetSlot : public QWidget
    {
        protected:
            WidgetSlot() : QWidget(nullptr) {}
    };

    class WidgetSlotFloat : public WidgetSlot
    {
        private:

        public:
            WidgetSlotFloat(float initialValue, const std::string &labelString,
                            ListInspectorItemWidget *parent);

            QDoubleSpinBox *spinbox;

            QSize sizeHint() const override
            {
                return QSize(50, 50);
            }
    };

    class WidgetSlotVectorFloat : public WidgetSlot
    {
        public:
            WidgetSlotVectorFloat(std::vector<float> initialValues, const std::string &labelString,
                             ListInspectorItemWidget *parent);

            std::vector<QDoubleSpinBox*> spinboxes;
    };

    //

    Part *relatedPart; //Set by ListInspector when creating it
    std::map<std::string, WidgetSlotVectorFloat*> labelsToSlotsVectorFloat;

public:
    explicit ListInspectorItemWidget(Part *relatedPart);
    std::vector<float> GetSlotValueVecFloat(const std::string &slotLabel);

public slots:
    void _NotifyInspectorSlotChanged(double newValue);
};

#endif // LISTINSPECTORITEM_H
