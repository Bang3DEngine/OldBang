#ifndef INSPECTORWIDGET_H
#define INSPECTORWIDGET_H

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
#include "InspectorWidgetInfo.h"

#include "IInspectable.h"
#include "IWindowEventManagerListener.h"

class Part;
class WindowEventManager;
class InspectorSW;
class InspectorWidget : public QWidget
{
    //Every part widget, has many slots,
    // representing all its attributes (vectors, strings, integers, etc.)
    Q_OBJECT

private:
    class SlotWidget;


    QLabel *titleLabel;
    std::vector<InspectorSW*> partSlots;
    std::map<std::string, InspectorSW*> labelsToPartSlots;

    QTimer *updateTimer;

protected:
    IInspectable *relatedInspectable; //Set by Inspector when creating it
    QHBoxLayout *titleLayout;

public:
    explicit InspectorWidget(Part *relatedPart);
    virtual ~InspectorWidget();

    std::vector<float> GetSWVectorFloatValue(const std::string &slotLabel);
    int GetSWSelectedEnumIndex(const std::string &slotLabel);
    std::string GetSWFileFilepath(const std::string &slotLabel);

public slots:

    virtual void OnCustomContextMenuRequested(QPoint point);

    void UpdateSlotsValues();

    void _NotifyInspectorSlotChanged(double _);
    void _NotifyInspectorSlotChanged(QString _);
    void _NotifyInspectorSlotChanged();
};

#endif // INSPECTORWIDGET_H
