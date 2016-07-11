#ifndef INSPECTORWIDGET_H
#define INSPECTORWIDGET_H

#include "Bang.h"

#include <string>
#include <iomanip>
#include <sstream>
#include <functional>

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


class Component;
class WindowEventManager;
class InspectorSW;
class InspectorWidget : public QWidget
{
    //Every comp widget, has many slots,
    // representing all its attributes (vectors, strings, integers, etc.)
    Q_OBJECT

private:
    //If not null, this function will be called when some slot value changes
    std::function<void()> *p_callback = nullptr;

    std::vector<InspectorSW*> m_compSlots;
    std::map<std::string, InspectorSW*> m_labelsToComponentSlots;

    QTimer *p_updateTimer = nullptr;

protected:
    IInspectable *p_relatedInspectable = nullptr;

    QHBoxLayout *p_titleLayout = nullptr;
    QLabel *p_titleLabel = nullptr;

    void ConstructFromWidgetInformation(const std::string &title,
                                        const InspectorWidgetInfo *info,
                                        bool autoUpdate = true);

public:
    InspectorWidget();
    explicit InspectorWidget(IInspectable *p_relatedInspectable);

    //Short handed way to use an Inspector Widget
    //(no need to create an IInspectable object).
    explicit InspectorWidget(const std::string &title,
                             InspectorWidgetInfo *widgetInfo,
                             std::function<void()> p_callback);

    virtual ~InspectorWidget();

    std::vector<float> GetSWVectorFloatValue(const std::string &slotLabel);
    int GetSWSelectedEnumIndex(const std::string &slotLabel);
    std::string GetSWFileFilepath(const std::string &slotLabel);

private slots:

    void Refresh();
    void _OnSlotValueChanged(double _);
    void _OnSlotValueChanged(QString _);

public slots:

    virtual void OnCustomContextMenuRequested(QPoint point);

    void Refresh(InspectorWidgetInfo *widgetInfo);
    virtual void _OnSlotValueChanged();
};

#endif // INSPECTORWIDGET_H
