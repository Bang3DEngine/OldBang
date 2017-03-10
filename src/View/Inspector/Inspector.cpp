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
#include "AudioSource.h"
#include "Application.h"
#include "EditorWindow.h"
#include "MeshRenderer.h"
#include "SceneManager.h"
#include "AudioListener.h"
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
    setMinimumWidth(300);

    horizontalScrollBar()->setEnabled(false);
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
    // Avoid double clearings
    ASSERT(!m_widget_To_Item.Empty());

    for (InspectorWidget *iw : m_currentInspectorWidgets) { iw->OnDestroy(); }
    m_currentInspectorWidgets.Clear();
    m_widget_To_Item.Clear();
    m_widget_To_Inspectables.Clear();
    m_currentInspectables.Clear();

    clear();

    m_titleLabel->setText(tr(""));
    m_currentGameObject = nullptr;
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
    RefreshSizeHints();
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

void Inspector::OnEditorPlay()
{
    Clear();
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

void Inspector::OnMenuBarAddNewBehaviourClicked()
{
    ASSERT(m_currentGameObject);
    BehaviourHolder *bh = m_currentGameObject->AddComponent<BehaviourHolder>();
    bh->CreateNewBehaviour();
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


String Inspector::FormatInspectorLabel(const String &labelString)
{
    String labelFormatted = labelString.Replace("_", " ");

    // Add space after every cap (except for consecutive caps).
    for (auto it = labelFormatted.Begin(); it != labelFormatted.End(); ++it)
    {
        ++it;
        bool nextOneIsCap = (it != labelFormatted.End()) && String::IsUpperCase(*it);
        --it;
        if (!nextOneIsCap && String::IsUpperCase(*it) && it != labelFormatted.Begin())
        {
            it = labelFormatted.insert(it, ' ');
            ++it;
        }
    }

    labelFormatted = labelFormatted;
    return labelFormatted.Replace("  ", " ");
}


void Inspector::OnGameObjectDestroyed(GameObject *destroyed)
{
    if (m_currentGameObject == destroyed)
    {
        Clear();
    }
}


