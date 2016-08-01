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

class Component;
class WindowEventManager;
class AttributeWidget;
/**
 * @brief Represents a widget that can be put in the Inspector.
 * It tracks the attributes and attributeWidget it contains.
 */
class InspectorWidget : public QWidget
{
    Q_OBJECT

private:

    /** @brief This variable is used to avoid premature OnSlotValueChanged, before creating the widget**/
    bool m_created = false;

    QTimer *m_updateTimer = nullptr;
    std::string m_tagName = "";
    std::vector<XMLAttribute> m_attributes;
    mutable std::map<std::string, AttributeWidget*> m_attrNameToAttrWidget;


    void CreateWidgetSlots(XMLNode &xmlInfo);
    void ConstructFromWidgetXMLInfo(const std::string &title, XMLNode &info, bool autoUpdate = true);

protected:
    IInspectable *m_relatedInspectable = nullptr;

    QHBoxLayout *m_titleLayout = nullptr;
    QLabel *m_titleLabel = nullptr;

public:
    InspectorWidget();
    explicit InspectorWidget(const std::string &title,
                             IInspectable *relatedInspectable);

    virtual ~InspectorWidget();

    /**
     * @brief GetUpdatedWidgetXMLInfo
     * @return An XMLNode with the information of the related IInspectable.
     *         It's not updated.
     */
    XMLNode GetInspectableXMLInfo() const;

    /**
     * @brief GetUpdatedWidgetXMLInfo
     * @return An XMLNode with the updated information
     */
    XMLNode GetWidgetXMLInfo() const;

private slots:

    /**
     * @brief Refreshes all the widget values with the related IInspectable current values.
     */
    void RefreshWidgetValues();

    void _OnSlotValueChanged(int _);
    void _OnSlotValueChanged(double _);
    void _OnSlotValueChanged(QString _);

public slots:

    virtual void OnCustomContextMenuRequested(QPoint point);

    virtual void _OnSlotValueChanged();
};

#endif // INSPECTORWIDGET_H
