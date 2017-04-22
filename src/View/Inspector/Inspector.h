#ifndef LISTINSPECTOR_H
#define LISTINSPECTOR_H

#include "Bang/Map.h"
#include "Bang/DragDropQListWidget.h"
#include "Bang/InspectorContextMenu.h"
#include "Bang/IWindowEventManagerListener.h"

class QLabel;
class QCheckBox;
class QDropEvent;
class SerializableObject;
class Inspector : public DragDropQListWidget,
                  public IWindowEventManagerListener
{
    Q_OBJECT

public:
    explicit Inspector(QWidget *parent = nullptr);

    void OnWindowShown();

    void Clear();

    void ShowInspectable(SerializableObject *inspectable,
                          const String &title = "");

    void OnEditorPlay();

public slots:
    void OnEnabledCheckBoxChanged(bool checked);
    void Refresh();

public:
    SerializableObject* GetCurrentInspectable();
    void OnSerializableObjectDestroyed(SerializableObject *destroyed);

    void AddWidget(InspectorWidget *widget, int row = -1);

    bool IsShowingInspectable(SerializableObject *inspectable) const;
    static Inspector* GetInstance();

    void dropEvent(QDropEvent *e) override;

    static String FormatInspectorLabel(const String &labelString);

protected:
    //To set the scroll step to a smaller one
    virtual void updateGeometries() override;

private:
    void RefreshSizeHints();

    InspectorContextMenu m_iContextMenu;

    QLabel *m_titleLabel = nullptr;
    QCheckBox *m_enabledCheckBox = nullptr;

    SerializableObject       *m_currentInspectable;
    List<InspectorWidget*>    m_currentInspectorWidgets;
    List<SerializableObject*> m_currentInspectableChildren;

    Map<InspectorWidget*, QListWidgetItem*> m_widget_To_Item;
    Map<InspectorWidget*, SerializableObject*> m_widget_To_Inspectables;

    friend class InspectorWidget;
    friend class InspectorContextMenu;
};

#endif // LISTINSPECTOR_H
