#ifndef LISTINSPECTOR_H
#define LISTINSPECTOR_H

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
class Inspector : public DragDropQListWidget,
                  public IDestroyListener
{
    Q_OBJECT

public:
    explicit Inspector(QWidget *parent = nullptr);

    void Clear();
    void OnWindowShown();
    void Show(Object *object);
    static Inspector* GetInstance();

    const List<InspectorWidget*> GetCurrentInspectorWidgets() const;
    void InsertInspectorWidget(InspectorWidget *inspectorWidget,
                               int row = -1);

public slots:
    void OnEnabledCheckBoxChanged(bool checked);
    bool Refresh();

protected:
    //To set the scroll step to a smaller one
    virtual void updateGeometries() override;

private:
    void Show(const List<InspectorWidget*> &inspectorWidgets);

    void dropEvent(QDropEvent *e) override;
    void RefreshSizeHints();

    List<InspectorWidget*> m_currentInspectorWidgets;
    InspectorContextMenu m_iContextMenu;

    QLabel    *m_titleLabel      = nullptr;
    QCheckBox *m_enabledCheckBox = nullptr;

protected: // IDestroyListener
    void OnDestroyDemanded(Destroyable *objectDemandingDestroy) override;

    friend class InspectorWidget;
    friend class InspectorContextMenu;
};

#endif // LISTINSPECTOR_H
