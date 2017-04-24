#ifndef LISTINSPECTOR_H
#define LISTINSPECTOR_H

#include "Bang/Map.h"
#include "Bang/DragDropQListWidget.h"
#include "Bang/InspectorContextMenu.h"

class QLabel;
class QCheckBox;
class QDropEvent;
class InspectorWidget;
class SerializableObject;
class Inspector : public DragDropQListWidget
{
    Q_OBJECT

public:
    explicit Inspector(QWidget *parent = nullptr);

    void OnWindowShown();

    void Clear();

    void RefreshInspectable(SerializableObject *serializableObject);
    void ShowInspectable(SerializableObject *inspectable,
                         const String &title = "");

public slots:
    void OnEnabledCheckBoxChanged(bool checked);
    void Refresh();

public:
    SerializableObject* GetCurrentInspectable();
    void OnSerializableObjectDestroyed(SerializableObject *destroyed);

    void AddWidget(InspectorWidget *widget, int row = -1);

    static Inspector* GetInstance();

    void dropEvent(QDropEvent *e) override;
    List<InspectorWidget*> GetCurrentInspectorWidgets() const;

    static String FormatInspectorLabel(const String &labelString);

protected:
    //To set the scroll step to a smaller one
    virtual void updateGeometries() override;

private:
    void RefreshSizeHints();

    InspectorContextMenu m_iContextMenu;

    QLabel    *m_titleLabel      = nullptr;
    QCheckBox *m_enabledCheckBox = nullptr;

    SerializableObject       *m_currentInspectable = nullptr;
    List<SerializableObject*> m_currentInspectableChildren;

    friend class InspectorWidget;
    friend class InspectorContextMenu;
};

#endif // LISTINSPECTOR_H
