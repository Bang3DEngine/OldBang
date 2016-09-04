#include "Inspector.h"

#include <QScrollBar>

#include "Debug.h"
#include "Camera.h"
#include "Component.h"
#include "Transform.h"
#include "PointLight.h"
#include "GameObject.h"
#include "Application.h"
#include "EditorWindow.h"
#include "MeshRenderer.h"
#include "CircleRenderer.h"
#include "BehaviourHolder.h"
#include "DirectionalLight.h"
#include "SingleLineRenderer.h"

Inspector::Inspector(QWidget *parent)
    : QListWidget(parent), m_iContextMenu(this)
{
    setAcceptDrops(true);
    setDragEnabled(true);
    setDropIndicatorShown(true);
    setAlternatingRowColors(true);
    viewport()->setAcceptDrops(true);
    setDefaultDropAction(Qt::DropAction::MoveAction);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setDragDropMode(QAbstractItemView::DragDropMode::DragDrop);

    m_titleLabel = parent->findChild<QLabel*>("labelInspectorGameObjectName");
    setMinimumWidth(360);
}

void Inspector::updateGeometries()
{
    QListWidget::updateGeometries();
    verticalScrollBar()->setSingleStep(3);
}

void Inspector::Clear()
{
    if (m_widget_To_Item.Empty()) return; // Avoid double clearings

    clear();

    m_widget_To_Item.Clear();
    m_currentGameObject = nullptr;
    m_titleLabel->setText(QString::fromStdString(""));

    for (InspectorWidget *iw : m_currentInspectorWidgets)
    {
        delete iw;
    }
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

void Inspector::SetInspectable(IInspectable *inspectable, const String &title)
{
    Clear();
    InspectorWidget *iw = new InspectorWidget();
    iw->Init(title, inspectable);
    m_widget_To_Inspectables[iw] = inspectable;
    m_currentInspectables.PushBack(inspectable);
    AddWidget(iw);
}

void Inspector::ShowGameObjectInfo(GameObject *gameObject)
{
    Clear();

    NONULL(gameObject);
    m_currentGameObject = gameObject;

    for (Component *c : gameObject->GetComponents())
    {
        ComponentWidget *w = new ComponentWidget(c);
        m_currentInspectables.PushBack(c);
        m_widget_To_Inspectables[w] = c;
        AddWidget(w);
    }

    m_titleLabel->setText(gameObject->name.ToQString());
    updateGeometry();
    adjustSize();
    update();
    show();
}

void Inspector::ShowPrefabInspectableInfo(PrefabAssetFileInspectable *prefabInspectable)
{
    ShowGameObjectInfo(prefabInspectable->GetPrefabTempGameObject());
    m_currentInspectables.PushBack(prefabInspectable);
}

void Inspector::RefreshHard()
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

void Inspector::AddWidget(InspectorWidget *widget, int row)
{
    NONULL(widget);

    int newRow = (row == -1 ? count() : row);

    QListWidgetItem *item = new QListWidgetItem();
    insertItem(newRow, item);

    m_widget_To_Item[widget] = item;
    m_currentInspectorWidgets.PushBack(widget);

    setItemWidget(item, widget);
    item->setSizeHint(widget->size());
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

void Inspector::OnMenuBarActionClicked(MenuBar::Action clickedAction)
{
    NONULL(m_currentGameObject);

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

    Refresh();
}

void Inspector::OnGameObjectDestroyed(GameObject *destroyed)
{
    if (m_currentGameObject == destroyed)
    {
        Clear();
    }
}


