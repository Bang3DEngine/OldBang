#ifndef LISTINSPECTOR_H
#define LISTINSPECTOR_H

#include <QLabel>
#include <QDropEvent>

#include "Map.h"
#include "DragDropAgent.h"
#include "InspectorContextMenu.h"
#include "PrefabAssetFileInspectable.h"
#include "IWindowEventManagerListener.h"

class Inspector : public DragDropQListWidget,
                  public IWindowEventManagerListener
{
    Q_OBJECT

public:
    explicit Inspector(QWidget *parent = nullptr);

    void OnWindowShown();

    void Clear();
    void Refresh();

    void MoveInspectorWidget(InspectorWidget *inspectorWidget, int movement);
    void SetInspectable(IInspectable *inspectable, const String &title = "");
    void ShowGameObjectInfo(GameObject *gameObject);

    void RefreshSizeHints();

public slots:
    void ShowCurrentGameObjectInfo();

    /**
     * @brief RefreshHard must be used when some hidden or non-hidden
     * attrWidgets are shown/hidden. Because of resizing reasons, we have
     * to reconstruct the layout in order for it to be propperly
     * renderered by Qt.
     * @param widget
     */
    void RefreshHard();

public:
    void ShowPrefabInspectableInfo(PrefabAssetFileInspectable *prefabInspectable);

    // IWindowEventManagerListener
    void OnMenuBarActionClicked(MenuBar::Action clickedAction) override;
    void OnGameObjectDestroyed(GameObject *destroyed);

    void AddWidget(InspectorWidget *widget, int row = -1);

    List<IInspectable*> GetCurrentInspectables() const;
    bool IsShowingInspectable(IInspectable *inspectable) const;
    static Inspector* GetInstance();

    void dropEvent(QDropEvent *e) override;

protected:
    //To set the scroll step to a smaller one
    virtual void updateGeometries() override;

private:
    InspectorContextMenu m_iContextMenu;

    QLabel *m_titleLabel = nullptr;
    GameObject *m_currentGameObject = nullptr;
    List<InspectorWidget*> m_currentInspectorWidgets;
    List<IInspectable*> m_currentInspectables;

    Map<InspectorWidget*, QListWidgetItem*> m_widget_To_Item;
    Map<InspectorWidget*, IInspectable*> m_widget_To_Inspectables;

    friend class InspectorWidget;
    friend class InspectorContextMenu;
};

#endif // LISTINSPECTOR_H
