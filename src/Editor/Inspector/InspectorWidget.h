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

class IInspectorWidget { };

class Component;
class GameObject;
class InspectorWidget : public DragDropQWidget,
                        public IInspectorWidget,
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

    void SetHeaderVisible(bool visible);
    bool IsHeaderVisible() const;

    virtual int GetHeightSizeHint() const;
    virtual void OnDestroy();

public slots:
    virtual void OnAttrWidgetValueChanged(IAttributeWidget *attrWidget);
    virtual void OnUpdate();

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
    List<AttributeWidget*> m_attributeWidgets;

    void UpdateContentMargins();

signals:
    void Changed(IInspectorWidget *inspectorWidget);
};

Q_DECLARE_METATYPE(IInspectorWidget)

#endif // INSPECTORWIDGET_H
