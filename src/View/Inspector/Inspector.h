#ifndef LISTINSPECTOR_H
#define LISTINSPECTOR_H

#include <map>

#include "Bang.h"

#include <QLabel>
#include <QListWidget>

#include "ComponentWidget.h"
#include "InspectorContextMenu.h"
#include "PrefabAssetFileInspectable.h"
#include "IWindowEventManagerListener.h"

class Inspector : public QListWidget,
                  public IWindowEventManagerListener
{
    Q_OBJECT

friend class InspectorContextMenu;

private:
    InspectorContextMenu m_iContextMenu;

    QLabel *m_titleLabel = nullptr;
    GameObject *m_currentGameObject = nullptr;
    List<InspectorWidget*> m_currentInspectorWidgets;
    List<IInspectable*> m_currentInspectables;

    std::map<InspectorWidget*, QListWidgetItem*> m_widgetToItem;
    std::map<InspectorWidget*, IInspectable*> m_widgetToInspectables;

protected:
    //To set the scroll step to a smaller one
    virtual void updateGeometries() override;

public:
    explicit Inspector(QWidget *parent = 0);

    void Clear();
    void Refresh();

    void SetInspectable(IInspectable *inspectable, const String &title = "");
    void ShowGameObjectInfo(GameObject *gameObject);
    void ShowPrefabInspectableInfo(PrefabAssetFileInspectable *prefabInspectable);

    // IWindowEventManagerListener
    void OnMenuBarActionClicked(MenuBar::Action clickedAction) override;
    void OnGameObjectDestroyed(GameObject *destroyed);

    /**
     * @brief RefreshHard must be used when some hidden or non-hidden
     * attrWidgets are shown/hidden. Because of resizing reasons, we have
     * to reconstruct the layout in order for it to be propperly
     * renderered by Qt.
     * @param widget
     */
    void RefreshHard();
    void AddWidget(InspectorWidget *widget, int row = -1);

    List<IInspectable*> GetCurrentInspectables() const;
    bool IsShowingInspectable(IInspectable *inspectable) const;
    static Inspector* GetInstance();
};

#endif // LISTINSPECTOR_H
