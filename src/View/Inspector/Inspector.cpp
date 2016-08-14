#include "Inspector.h"

#include <QScrollBar>

#include "Logger.h"
#include "Camera.h"
#include "Component.h"
#include "Transform.h"
#include "PointLight.h"
#include "WindowMain.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "CircleRenderer.h"
#include "BehaviourHolder.h"
#include "DirectionalLight.h"
#include "SingleLineRenderer.h"

Inspector::Inspector(QWidget *parent) : QListWidget(parent)
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
    setMinimumWidth(300);
}

void Inspector::updateGeometries()
{
    QListWidget::updateGeometries();
    verticalScrollBar()->setSingleStep(3);
}

void Inspector::Clear()
{
    if (m_widgetToItem.size() == 0) return; // Avoid double clearings

    clear();

    m_widgetToItem.clear();
    m_currentGameObject = nullptr;
    m_titleLabel->setText(QString::fromStdString(""));

    for (InspectorWidget *iw : m_currentInspectorWidgets)
    {
        delete iw;
    }
    m_currentInspectorWidgets.clear();
    m_widgetToInspectables.clear();
    m_currentInspectables.clear();
}

void Inspector::Refresh()
{
    if (m_currentGameObject)
    {
        ShowGameObjectInfo(m_currentGameObject);
        updateGeometry();
        adjustSize();
    }
}

void Inspector::SetInspectable(IInspectable *inspectable, const std::string &title)
{
    Clear();
    InspectorWidget *iw = new InspectorWidget();
    iw->Init(title, inspectable);
    m_widgetToInspectables[iw] = inspectable;
    m_currentInspectables.push_back(inspectable);
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
        m_currentInspectables.push_back(c);
        m_widgetToInspectables[w] = c;
        AddWidget(w);
    }

    m_titleLabel->setText(QString(gameObject->name.c_str()));
}

void Inspector::ShowPrefabInspectableInfo(PrefabAssetFileInspectable *prefabInspectable)
{
    ShowGameObjectInfo(prefabInspectable->GetPrefabTempGameObject());
    m_currentInspectables.push_back(prefabInspectable);
}

void Inspector::RefreshHard()
{
    if (m_currentGameObject)
    {
        GameObject *go = m_currentGameObject;
        ShowGameObjectInfo(go);
    }
    else
    {
        if(m_currentInspectables.size() == 1)
        {
            IInspectable *insp = m_currentInspectables[0];
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

    m_widgetToItem[widget] = item;
    m_currentInspectorWidgets.push_back(widget);

    setItemWidget(item, widget);
    item->setSizeHint(widget->size());
}



void Inspector::MoveUp(InspectorWidget *w)
{
    int lastRow = row(m_widgetToItem[w]);
    if (lastRow == 0) return;
    QListWidgetItem *item = takeItem(lastRow);
    insertItem(lastRow - 1, item);
}

void Inspector::MoveDown(InspectorWidget *w)
{
    int lastRow = row(m_widgetToItem[w]);
    if (lastRow == count()) return;
    QListWidgetItem *item = takeItem(lastRow);
    insertItem(lastRow + 1, item);
}

std::vector<IInspectable *> Inspector::GetCurrentInspectables() const
{
    return m_currentInspectables;
}

bool Inspector::IsShowingInspectable(IInspectable *inspectable) const
{
    for (IInspectable *insp : m_currentInspectables)
    {
        if (inspectable == insp)
        {
            return true;
        }
    }
    return false;
}

Inspector *Inspector::GetInstance()
{
    return WindowMain::GetInstance()->widgetInspector;
}

void Inspector::OnMenuBarActionClicked(MenuBar::Action clickedAction)
{
    NONULL(m_currentGameObject);

    if (clickedAction == MenuBar::Action::AddComponentBehaviour)
    {
        BehaviourHolder *bh = new BehaviourHolder();
        m_currentGameObject->AddComponent(bh);
    }
    else if (clickedAction == MenuBar::Action::AddComponentCamera)
    {
        Camera *c = new Camera();
        m_currentGameObject->AddComponent(c);
    }
    else if (clickedAction == MenuBar::Action::AddComponentMeshRenderer)
    {
        MeshRenderer *m = new MeshRenderer();
        m_currentGameObject->AddComponent(m);
    }
    else if (clickedAction == MenuBar::Action::AddComponenSingleLineRenderer)
    {
        SingleLineRenderer *slr = new SingleLineRenderer();
        m_currentGameObject->AddComponent(slr);
    }
    else if (clickedAction == MenuBar::Action::AddComponentCircleRenderer)
    {
        CircleRenderer *cr = new CircleRenderer();
        m_currentGameObject->AddComponent(cr);
    }
    else if (clickedAction == MenuBar::Action::AddComponentDirectionalLight)
    {
        DirectionalLight *dl = new DirectionalLight();
        m_currentGameObject->AddComponent(dl);
    }
    else if (clickedAction == MenuBar::Action::AddComponentPointLight)
    {
        PointLight *pl = new PointLight();
        m_currentGameObject->AddComponent(pl);
    }

    Refresh();
}


