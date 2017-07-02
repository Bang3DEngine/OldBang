#ifndef INSPECTORWIDGET_H
#define INSPECTORWIDGET_H

#include <iomanip>
#include <sstream>

#include <QLabel>
#include <QTimer>
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
#include "Bang/WinUndef.h"

#include "Bang/File.h"
#include "Bang/Debug.h"
#include "Bang/XMLNode.h"
#include "Bang/DragDropQWidget.h"
#include "Bang/SerializableObject.h"

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
    void Init(SerializableObject *relatedInspectable);

    virtual ~InspectorWidget();

    /**
     * @brief GetUpdatedWidgetXMLInfo
     * @return An XMLNode with the info of the related SerializableObject.
     *         It's not updated.
     */
    XMLNode GetInspectableXMLInfo() const;

    /**
     * @brief GetUpdatedWidgetXMLInfo
     * @return An XMLNode with the updated information
     */
    XMLNode GetWidgetXMLInfo() const;

    SerializableObject* GetRelatedInspectable() const;

    QGridLayout *GetGridLayout();
    int GetNextRowIndex() const;

    void SetTitle(const String &title);
    bool IsClosed() const;

    virtual int GetHeightSizeHint();
    void OnDestroy();

public slots:

    virtual void _OnSlotValueChanged();
    /**
     * @brief Refreshes all widget values with the related SerializableObject
     * current values.
     */
    void RefreshWidgetValues();

protected:
    SerializableObject *p_relatedSerialObj = nullptr;

    QVBoxLayout m_vLayout;
    QHBoxLayout m_headerLayout;
    QToolButton m_closeOpenButton;
    QLabel m_iconLabel;
    QLabel m_titleLabel;
    QGridLayout m_gridLayout;

    bool m_closed = false;

    void SetIcon(const QPixmap &icon);
    void UpdateCloseOpenButtonIcon();
    virtual void SetClosed(bool closedWidget);

protected slots:
    void OnCloseOpenButtonClicked();

private:
    /** @brief This variable is used to avoid premature OnSlotValueChanged,
     * before creating the widget**/
    bool m_created = false;

    String m_tagName = "";
    QTimer m_refreshTimer;
    Array<XMLAttribute> m_attributes;
    mutable Map<String, AttributeWidget*> m_attrName_To_AttrWidget;


    void UpdateContentMargins();
    void CreateWidgetSlots(XMLNode &xmlInfo);
    void ConstructFromWidgetXMLInfo(XMLNode &info);

private slots:
    void _OnSlotValueChanged(int _);
    void _OnSlotValueChanged(bool _);
    void _OnSlotValueChanged(double _);
    void _OnSlotValueChanged(QString _);
};

#endif // INSPECTORWIDGET_H
