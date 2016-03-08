#ifndef LISTINSPECTORITEMWIDGET_H
#define LISTINSPECTORITEMWIDGET_H

#include "Bang.h"

#include <string>
#include <iomanip>
#include <sstream>

#include <QLabel>
#include <QLayout>
#include <QWidget>
#include <QCheckBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QListWidget>
#include <QDoubleSpinBox>

#include "Logger.h"
#include "InspectorPartInfo.h"
#include "InspectorPartSlotWidget.h"

#include "IWindowEventManagerListener.h"

class Part;
class WindowEventManager;
class InspectorPartWidget : public QWidget
{
    //Every part widget, has many slots,
    // representing all relatedPart's attributes (vectors, strings, integers, etc.)
    Q_OBJECT

private:
    class SlotWidget;

    Part *relatedPart; //Set by Inspector when creating it
    QLabel *titleLabel;
    QCheckBox *enabledCheckbox;

    //void=InspectorPartSlotWidget<T>*
    std::map<std::string, void*> labelsToPartSlots;

    QTimer *updateTimer;

public:
    explicit InspectorPartWidget(Part *relatedPart);
    virtual ~InspectorPartWidget();

    template <class T>
    T GetSlotValue(const std::string &slotLabel)
    {
        InspectorPartSlotWidget<T> *w =
                static_cast<InspectorPartSlotWidget<T>*>(labelsToPartSlots[slotLabel]);
        return *( w->GetValue() );
    }

public slots:
    void UpdateSlotsValues();
    void OnEnabledCheckboxPressed(bool checked);
    void _NotifyInspectorSlotChanged(double _);
    void _NotifyInspectorSlotChanged(QString _);
};

#endif // LISTINSPECTORITEM_H
