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

class Component;
class GameObject;
class IInspectable;
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
    void Init(IInspectable *relatedInspectable);

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

    IInspectable* GetInspectable() const;
    QGridLayout *GetGridLayout();

    void SetTitle(const String &title);
    bool IsClosed() const;

    virtual int GetHeightSizeHint() const;
    void OnDestroy();

public slots:

    virtual void OnAttrWidgetValueChanged();
    /**
     * @brief Refreshes all widget values with the related SerializableObject
     * current values.
     */
    void Refresh();

protected:
    IInspectable *p_inspectable = nullptr;

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

    virtual void CreateWidgetSlots(const XMLNode &xmlInfo);

protected slots:
    void OnCloseOpenButtonClicked();

private:
    /** @brief This variable is used to avoid premature OnSlotValueChanged,
     * before creating the widget**/
    bool m_created = false;

    String m_tagName = "";
    QTimer m_refreshTimer;
    Map<AttributeWidget*, XMLAttribute> m_attrWidget_To_XMLAttr;
    mutable Map<String, AttributeWidget*> m_attrName_To_AttrWidget;

    void UpdateContentMargins();
    void ConstructFromWidgetXMLInfo(const XMLNode &info);
};

#endif // INSPECTORWIDGET_H
