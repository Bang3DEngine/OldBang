#ifndef INSPECTORWIDGET_H
#define INSPECTORWIDGET_H

#include <iomanip>
#include <sstream>

#include <QLabel>
#include <QLayout>
#include <QWidget>
#include <QCheckBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QToolButton>
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

public slots:

    virtual void _OnSlotValueChanged();
    /**
     * @brief Refreshes all the widget values with the related IInspectable current values.
     */
    void RefreshWidgetValues();

protected:
    IInspectable *m_relatedInspectable = nullptr;

    QVBoxLayout *m_vLayout = nullptr;
        QHBoxLayout *m_header = nullptr;
            QToolButton *m_closeOpenButton = nullptr;
            QPixmap m_openButtonPixmapap, m_closeButtonPixmap;
            QLabel *m_titleLabel = nullptr;
    QGridLayout *m_gridLayout = nullptr;

    bool m_closed = false;

protected slots:
    void OnCloseOpenButtonClicked();

protected:
    void UpdateCloseOpenButtonIcon();
    virtual void SetClosed(bool closedWidget);

private:
    /** @brief This variable is used to avoid premature OnSlotValueChanged, before creating the widget**/
    bool m_created = false;

    String m_tagName = "";
    QTimer m_updateTimer;
    Array<XMLAttribute> m_attributes;
    mutable Map<String, AttributeWidget*> m_attrName_To_AttrWidget;


    void CreateWidgetSlots(XMLNode &xmlInfo);
    void ConstructFromWidgetXMLInfo(const String &title,
                                    XMLNode &info, bool autoUpdate = true);

private slots:
    void _OnSlotValueChanged(int _);
    void _OnSlotValueChanged(double _);
    void _OnSlotValueChanged(QString _);
};

#endif // INSPECTORWIDGET_H
