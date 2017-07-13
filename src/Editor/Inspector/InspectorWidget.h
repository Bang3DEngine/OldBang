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

#include "Bang/Map.h"
#include "Bang/File.h"
#include "Bang/Destroyable.h"
#include "Bang/AttrWidgetInt.h"
#include "Bang/AttrWidgetBool.h"
#include "Bang/AttrWidgetEnum.h"
#include "Bang/AttrWidgetFile.h"
#include "Bang/AttributeWidget.h"
#include "Bang/AttrWidgetFloat.h"
#include "Bang/AttrWidgetColor.h"
#include "Bang/DragDropQWidget.h"
#include "Bang/AttrWidgetString.h"
#include "Bang/AttrWidgetButton.h"
#include "Bang/AttrWidgetVectorFloat.h"

class Component;
class GameObject;
/**
 * @brief Represents a widget that can be put in the Inspector.
 * It tracks the attributes and attributeWidget it contains.
 */
class InspectorWidget : public DragDropQWidget,
                        public Destroyable
{
    Q_OBJECT

public:
    InspectorWidget();
    virtual ~InspectorWidget();

    void Init();

    QGridLayout *GetGridLayout();

    void SetTitle(const String &title);
    bool IsClosed() const;

    virtual int GetHeightSizeHint() const;
    virtual void OnDestroy();

public slots:
    virtual void OnAttrWidgetValueChanged(IAttributeWidget *attrWidget);

    /**
     * @brief Refreshes all widget values with the related SerializableObject
     * current values.
     */
    virtual void Refresh();

protected:
    QVBoxLayout m_vLayout;
    QHBoxLayout m_headerLayout;
    QToolButton m_closeOpenButton;
    QLabel m_iconLabel;
    QLabel m_titleLabel;
    QGridLayout m_gridLayout;

    bool m_closed = false;

    virtual void InitExtra();
    void SetIcon(const QPixmap &icon);
    void UpdateCloseOpenButtonIcon();
    virtual void SetClosed(bool closedWidget);

    void InsertAttributeWidget(AttributeWidget *attrWidget, int index = -1);

protected slots:
    void OnCloseOpenButtonClicked();

private:
    /** @brief This variable is used to avoid premature OnValueChanged,
     * before creating the widget**/
    bool m_created = false;

    String m_tagName = "";
    QTimer m_refreshTimer;
    List<AttributeWidget*> m_attributeWidgets;
    mutable Map<String, AttributeWidget*> m_attrName_To_AttrWidget;

    void UpdateContentMargins();
};

#endif // INSPECTORWIDGET_H
