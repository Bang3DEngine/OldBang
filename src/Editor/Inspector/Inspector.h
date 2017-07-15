#ifndef INSPECTOR_H
#define INSPECTOR_H

#include <QTimer>
#include "Bang/WinUndef.h"

#include "Bang/Map.h"
#include "Bang/Destroyable.h"
#include "Bang/DragDropQListWidget.h"
#include "Bang/InspectorContextMenu.h"

class QLabel;
class Object;
class QCheckBox;
class QDropEvent;
class InspectorWidget;
class SerializableObject;
class InspectorWidgetGroup;
class Inspector : public DragDropQListWidget,
                  public IDestroyListener
{
    Q_OBJECT

public:
    static Inspector* GetInstance();
    explicit Inspector(QWidget *parent = nullptr);

    void Clear();
    void OnWindowShown();
    void Show(Object *object, bool clearWhenDestroyed);
    Object *GetInspectedObject() const;

    const List<InspectorWidget*>& GetCurrentInspectorWidgets() const;
    const InspectorWidgetGroup* GetCurrentInspectorWidgetGroup() const;
    void InsertInspectorWidget(InspectorWidget *inspectorWidget,
                               int row = -1);

public slots:
    void OnEnabledCheckBoxChanged(bool checked);
    void OnUpdate();

protected:
    //To set the scroll step to a smaller one
    virtual void updateGeometries() override;

private:
    InspectorWidgetGroup* m_currentInspectorWidgetGroup = nullptr;
    InspectorContextMenu m_iContextMenu;

    QTimer     m_refreshTimer;
    Object    *p_inspectedObject = nullptr;
    QLabel    *m_titleLabel      = nullptr;
    QCheckBox *m_enabledCheckBox = nullptr;

    void Show(InspectorWidgetGroup* inspectorWidgetGroup);

    void dropEvent(QDropEvent *e) override;
    void RefreshSizeHints();

protected: // IDestroyListener
    void OnDestroyableDestroyed(Destroyable *destroyedObject) override;
    void OnDestroyDemanded(Destroyable *objectDemandingDestroy) override;

    friend class InspectorWidget;
    friend class InspectorContextMenu;
};

#endif // INSPECTOR_H
