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
        public:
            WidgetSlotFloat(float initialValue, const std::string &labelString,
                            ListInspectorItemWidget *parent);

            QSize sizeHint() const override
            {
                return QSize(50, 50);
            }
    };

    class WidgetSlotVector : public WidgetSlot
    {
        public:
            WidgetSlotVector(std::vector<float> initialValues, const std::string &labelString,
                             ListInspectorItemWidget *parent);
    };

    //

    Part *relatedPart; //Set by ListInspector when creating it


public:
    explicit ListInspectorItemWidget(Part *relatedPart);

public slots:
    void _NotifyInspectorSlotChanged(double newValue);
};

#endif // LISTINSPECTORITEM_H
