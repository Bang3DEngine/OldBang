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
#include "XMLNode.h"

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
    std::function<void()> *m_callback = nullptr;

    std::vector<InspectorSW*> m_compSlots;
    std::map<std::string, InspectorSW*> m_attrNameToComponentSlots;

    QTimer *m_updateTimer = nullptr;

protected:
    IInspectable *m_relatedInspectable = nullptr;

    QHBoxLayout *m_titleLayout = nullptr;
    QLabel *m_titleLabel = nullptr;

    void ConstructFromWidgetXMLInfo(const std::string &title,
                                    XMLNode &info,
                                    bool autoUpdate = true);

public:
    InspectorWidget();
    explicit InspectorWidget(IInspectable *relatedInspectable);

    //Short handed way to use an Inspector Widget
    //(no need to create an IInspectable object).
    explicit InspectorWidget(const std::string &title,
                             XMLNode &widgetXMLInfo,
                             std::function<void()> m_callback);

    virtual ~InspectorWidget();

    std::vector<float> GetSWVectorFloatValue(const std::string &slotLabel);
    int GetSWSelectedEnumIndex(const std::string &slotLabel);
    std::string GetSWFileFilepath(const std::string &slotLabel);

private slots:

    void CreateWidgetSlots(XMLNode &xmlInfo);
    void RefreshWidgetValues();
    void _OnSlotValueChanged(double _);
    void _OnSlotValueChanged(QString _);

public slots:

    virtual void OnCustomContextMenuRequested(QPoint point);

    void RefreshWidgetValues(XMLNode &xmlInfo);
    virtual void _OnSlotValueChanged();
};

#endif // INSPECTORWIDGET_H
