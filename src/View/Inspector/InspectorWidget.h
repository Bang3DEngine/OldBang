#ifndef INSPECTORWIDGET_H
#define INSPECTORWIDGET_H

#include "Bang.h"


#include <iomanip>
#include <sstream>
#include <functional>

#include <QLabel>
#include <QLayout>
#include <QWidget>
#include <QCheckBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QFormLayout>
#include <QPushButton>
#include <QListWidget>
#include <QDoubleSpinBox>

#include "File.h"
#include "Debug.h"
#include "XMLNode.h"
#include "IInspectable.h"
#include "DragDropAgent.h"

class Component;
class GameObject;
class AttributeWidget;

/**
 * @brief Represents a widget that can be put in the Inspector.
 * It tracks the attributes and attributeWidget it contains.
 */
class InspectorWidget : public DragDropQWidget
{
    Q_OBJECT

private:

    /** @brief This variable is used to avoid premature OnSlotValueChanged, before creating the widget**/
    bool m_created = false;

    QTimer *m_updateTimer = nullptr;
    String m_tagName = "";
    std::vector<XMLAttribute> m_attributes;
    mutable std::map<String, AttributeWidget*> m_attrNameToAttrWidget;


    void CreateWidgetSlots(XMLNode &xmlInfo);
    void ConstructFromWidgetXMLInfo(const String &title,
                                    XMLNode &info, bool autoUpdate = true);

protected:
    IInspectable *m_relatedInspectable = nullptr;

    QGridLayout *m_gridLayout = nullptr;
    QLabel *m_titleLabel = nullptr;

public:
    InspectorWidget();
    void Init(const String &title,
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

    QGridLayout *GetGridLayout() const;
    int GetNextRowIndex() const;

    void SetTitle(const String &title);

private slots:

    /**
     * @brief Refreshes all the widget values with the related IInspectable current values.
     */
    void RefreshWidgetValues();

    void _OnSlotValueChanged(int _);
    void _OnSlotValueChanged(double _);
    void _OnSlotValueChanged(QString _);

public slots:

    virtual void _OnSlotValueChanged();
};

#endif // INSPECTORWIDGET_H
