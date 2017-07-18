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
class IInspectorWidget;
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

public slots:
    void InsertInspectorWidget(InspectorWidget *inspectorWidget);
    void MoveInspectorWidget(InspectorWidget *inspectorWidget, int newRow);
    void RemoveInspectorWidget(InspectorWidget *inspectorWidget);
    void OnEnabledCheckBoxChanged(bool checked);
    void OnUpdate();

protected:
    //To set the scroll step to a smaller one
    virtual void updateGeometries() override;

private:
    InspectorWidgetGroup* m_currentInspectorWidgetGroup = nullptr;
    InspectorContextMenu m_iContextMenu;

    QTimer     m_updateTimer;
    Object    *p_inspectedObject = nullptr;
    QLabel    *m_titleLabel      = nullptr;
    QCheckBox *m_enabledCheckBox = nullptr;

    void Show(InspectorWidgetGroup* inspectorWidgetGroup);
    int GetInspectorWidgetRow(InspectorWidget *inspectorWidget) const;

    void dropEvent(QDropEvent *e) override;
    void UpdateSizeHints();

protected: // IDestroyListener
    void OnDestroyableDestroyed(Destroyable *destroyedObject) override;

    friend class InspectorWidget;
};

#endif // INSPECTOR_H
