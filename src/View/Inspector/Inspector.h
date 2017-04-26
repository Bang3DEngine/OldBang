#ifndef LISTINSPECTOR_H
#define LISTINSPECTOR_H

#include "Bang/Map.h"
#include "Bang/DragDropQListWidget.h"
#include "Bang/InspectorContextMenu.h"

class QLabel;
class QCheckBox;
class QDropEvent;
class IInspectable;
class InspectorWidget;
class SerializableObject;
class Inspector : public DragDropQListWidget
{
    Q_OBJECT

public:
    explicit Inspector(QWidget *parent = nullptr);

    void OnWindowShown();

    void Clear();

    bool Refresh(SerializableObject *serialObject);
    void ShowInspectable(IInspectable *inspectable);

public slots:
    void OnEnabledCheckBoxChanged(bool checked);
    void Refresh();

public:
    IInspectable* GetCurrentInspectable();
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

    IInspectable *m_currentInspectable = nullptr;

    friend class InspectorWidget;
    friend class InspectorContextMenu;
};

#endif // LISTINSPECTOR_H
