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

#include "IWindowEventManagerListener.h"

class Part;
class WindowEventManager;
class InspectorPartSW;
class InspectorPartWidget : public QWidget
{
    //Every part widget, has many slots,
    // representing all its attributes (vectors, strings, integers, etc.)
    Q_OBJECT

private:
    class SlotWidget;

    Part *relatedPart; //Set by Inspector when creating it
    QLabel *titleLabel;
    QCheckBox *enabledCheckbox;
    std::vector<InspectorPartSW*> partSlots;
    std::map<std::string, InspectorPartSW*> labelsToPartSlots;

    QTimer *updateTimer;

public:
    explicit InspectorPartWidget(Part *relatedPart);
    virtual ~InspectorPartWidget();

    std::vector<float> GetSWVectorFloatValue(const std::string &slotLabel);
    int GetSWSelectedEnumIndex(const std::string &slotLabel);
    std::string GetSWFileFilepath(const std::string &slotLabel);

public slots:

    void OnCustomContextMenuRequested(QPoint point);

    void OnContextMenuRemovePartSelected();
    void OnContextMenuMoveUpSelected();
    void OnContextMenuMoveDownSelected();

    void UpdateSlotsValues();
    void OnEnabledCheckboxPressed(bool checked);
    void _NotifyInspectorSlotChanged(double _);
    void _NotifyInspectorSlotChanged(QString _);
    void _NotifyInspectorSlotChanged();
};

#endif // LISTINSPECTORITEM_H
