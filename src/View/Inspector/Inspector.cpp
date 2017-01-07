#include "Inspector.h"

#include <QScrollBar>

#include "Scene.h"
#include "Debug.h"
#include "UIText.h"
#include "Canvas.h"
#include "Camera.h"
#include "UIImage.h"
#include "Component.h"
#include "Transform.h"
#include "PointLight.h"
#include "GameObject.h"
#include "Application.h"
#include "EditorWindow.h"
#include "MeshRenderer.h"
#include "SceneManager.h"
#include "CircleRenderer.h"
#include "ComponentWidget.h"
#include "BehaviourHolder.h"
#include "DirectionalLight.h"
#include "SingleLineRenderer.h"

Inspector::Inspector(QWidget *parent)
    : DragDropQListWidget(), m_iContextMenu(this)
{
    setAlternatingRowColors(true);
    setSelectionMode(QAbstractItemView::SingleSelection);

    m_titleLabel = parent->findChild<QLabel*>("labelInspectorGameObjectName");
    //setMinimumWidth(380);
    setMinimumWidth(300);
}

void Inspector::OnWindowShown()
{
    this->SetDragDropEventPossibleSources({
          Explorer::GetInstance(), Hierarchy::GetInstance()});
}

void Inspector::updateGeometries()
{
    QListWidget::updateGeometries();
    verticalScrollBar()->setSingleStep(3);
}

void Inspector::Clear()
{
    ASSERT(!m_widget_To_Item.Empty()); // Avoid double clearings

    clear();

    m_widget_To_Item.Clear();
    m_currentGameObject = nullptr;
    m_titleLabel->setText(tr(""));

    m_currentInspectorWidgets.Clear();
    m_widget_To_Inspectables.Clear();
    m_currentInspectables.Clear();
}

void Inspector::Refresh()
{
    if (m_currentGameObject)
    {
        ShowGameObjectInfo(m_currentGameObject);
    }
    else
    {
        if(m_currentInspectables.Size() == 1)
        {
            IInspectable *insp = m_currentInspectables.Front();
            SetInspectable(insp);
        }
    }
}

void Inspector::MoveInspectorWidget(InspectorWidget *inspectorWidget, int movement)
{
    QListWidgetItem *movingItem = m_widget_To_Item[inspectorWidget];
    ASSERT(movingItem);

    const int itemCount = count();
    const int oldRow = row(movingItem);
    ASSERT(oldRow >= 0 && oldRow < itemCount);

    takeItem(oldRow);
    const int newRow = (oldRow + movement + itemCount) % itemCount;
    insertItem(newRow, movingItem);
}

void Inspector::SetInspectable(IInspectable *inspectable, const String &title)
{
    Clear();
    InspectorWidget *iw = new InspectorWidget();
    iw->Init(title, inspectable);
    m_widget_To_Inspectables[iw] = inspectable;
    m_currentInspectables.PushBack(inspectable);
    AddWidget(iw);
    RefreshSizeHints();
}

void Inspector::ShowGameObjectInfo(GameObject *gameObject)
{
    Debug_Log("ShowGameObjectInfo " << gameObject->name);

    Clear();

    ASSERT(gameObject);
    m_currentGameObject = gameObject;

    for (Component *c : gameObject->GetComponents())
    {
        ComponentWidget *w = new ComponentWidget(c);
        m_currentInspectables.PushBack(c);
        m_widget_To_Inspectables[w] = c;
        w->RefreshWidgetValues();
        AddWidget(w);
    }

    RefreshSizeHints();
    m_titleLabel->setText(gameObject->name.ToQString());
}

void Inspector::RefreshSizeHints()
{
    for (InspectorWidget *iw : m_currentInspectorWidgets)
    {
        QListWidgetItem *item = m_widget_To_Item[iw];
        int width = iw->size().width();
        const int height = iw->GetHeightSizeHint();
        item->setSizeHint( QSize(width, height) );
    }
}

void Inspector::ShowCurrentGameObjectInfo()
{
    ShowGameObjectInfo(m_currentGameObject);
}

void Inspector::ShowPrefabInspectableInfo(PrefabAssetFileInspectable *prefabInspectable)
{
    ShowGameObjectInfo(prefabInspectable->GetPrefabTempGameObject());
    m_currentInspectables.PushBack(prefabInspectable);
}

void Inspector::RefreshHard()
{
    Debug_Clear();
    Debug_Log("RefreshHard " << std::rand());
    if (m_currentGameObject)
    {
        ShowGameObjectInfo(m_currentGameObject);
        update();
        updateGeometry();
        repaint();
        adjustSize();
    }
    else
    {
        if(m_currentInspectables.Size() == 1)
        {
            IInspectable *insp = m_currentInspectables.Front();
            SetInspectable(insp);
        }
    }
}

void Inspector::AddWidget(InspectorWidget *widget, int row)
{
    ASSERT(widget);

    int newRow = (row == -1 ? count() : row);

    QListWidgetItem *item = new QListWidgetItem();
    insertItem(newRow, item);

    m_widget_To_Item[widget] = item;
    m_currentInspectorWidgets.PushBack(widget);

    setItemWidget(item, widget);
}

List<IInspectable *> Inspector::GetCurrentInspectables() const
{
    return m_currentInspectables;
}

bool Inspector::IsShowingInspectable(IInspectable *inspectable) const
{
    return m_currentInspectables.Contains(inspectable);
}

Inspector *Inspector::GetInstance()
{
    return EditorWindow::GetInstance()->widgetInspector;
}

void Inspector::dropEvent(QDropEvent *e)
{
    e->ignore();
}

void Inspector::OnMenuBarActionClicked(MenuBar::Action clickedAction)
{
    ASSERT(m_currentGameObject);

    if (clickedAction == MenuBar::Action::AddComponentBehaviour)
    {
        BehaviourHolder *bh = m_currentGameObject->AddComponent<BehaviourHolder>();
    }
    else if (clickedAction == MenuBar::Action::AddComponentCamera)
    {
        Camera *c = m_currentGameObject->AddComponent<Camera>();
    }
    else if (clickedAction == MenuBar::Action::AddComponentMeshRenderer)
    {
        MeshRenderer *m = m_currentGameObject->AddComponent<MeshRenderer>();
    }
    else if (clickedAction == MenuBar::Action::AddComponenSingleLineRenderer)
    {
        SingleLineRenderer *slr = m_currentGameObject->AddComponent<SingleLineRenderer>();
    }
    else if (clickedAction == MenuBar::Action::AddComponentCircleRenderer)
    {
        CircleRenderer *cr = m_currentGameObject->AddComponent<CircleRenderer>();
    }
    else if (clickedAction == MenuBar::Action::AddComponentDirectionalLight)
    {
        DirectionalLight *dl = m_currentGameObject->AddComponent<DirectionalLight>();
    }
    else if (clickedAction == MenuBar::Action::AddComponentPointLight)
    {
        PointLight *pl = m_currentGameObject->AddComponent<PointLight>();
    }
    else if (clickedAction == MenuBar::Action::AddComponentCanvas)
    {
        Canvas *c = m_currentGameObject->AddComponent<Canvas>();
    }
    else if (clickedAction == MenuBar::Action::AddComponentUIImage)
    {
        UIImage *img = m_currentGameObject->AddComponent<UIImage>();
    }
    else if (clickedAction == MenuBar::Action::AddComponentUIText)
    {
        UIText *text = m_currentGameObject->AddComponent<UIText>();
    }

    Refresh();
}

void Inspector::OnGameObjectDestroyed(GameObject *destroyed)
{
    if (m_currentGameObject == destroyed)
    {
        Clear();
    }
}


