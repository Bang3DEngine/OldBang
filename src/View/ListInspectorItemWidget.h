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

class ListInspectorItemWidget : public QWidget
{
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
        public:
            WidgetSlotFloat(float initialValue = 0.0f, const std::string &labelString = "");
            QSize sizeHint() const override
            {
                return QSize(50, 50);
            }
    };

    class WidgetSlotVector : public WidgetSlot
    {
        public:
            WidgetSlotVector(std::vector<float> initialValues, const std::string &labelString = "");
    };

    //


public:
    explicit ListInspectorItemWidget(const std::string &title, ListInspectorItemInfo &itemInfo);
};

#endif // LISTINSPECTORITEM_H
