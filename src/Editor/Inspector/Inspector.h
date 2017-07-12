#ifndef LISTINSPECTOR_H
#define LISTINSPECTOR_H

#include "Bang/Map.h"
#include "Bang/DragDropQListWidget.h"
#include "Bang/InspectorContextMenu.h"

class QLabel;
class Component;
class QCheckBox;
class QDropEvent;
class InspectorWidget;
class SerializableObject;
class Inspector : public DragDropQListWidget
{
    Q_OBJECT

public:
    explicit Inspector(QWidget *parent = nullptr);

    void Clear();
    void OnWindowShown();
    void Show(GameObject *gameObject);
    void Show(Component *component);
    void Show(const BFile &file);
    static Inspector* GetInstance();

    const List<InspectorWidget*> GetCurrentInspectorWidgets() const;
    void OnSerializableObjectDestroyed(SerializableObject *destroyed);
    void InsertInspectorWidget(InspectorWidget *inspectorWidget,
                               int row = -1);
    static String FormatInspectorLabel(const String &labelString);

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

    friend class InspectorWidget;
    friend class InspectorContextMenu;
};

#endif // LISTINSPECTOR_H
