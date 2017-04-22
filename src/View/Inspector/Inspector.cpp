#include "Bang/Inspector.h"

#include <QLabel>
#include <QDropEvent>
#include <QScrollBar>
#include "Bang/WinUndef.h"

#include "Bang/EditorWindow.h"
#include "Bang/InspectorWidget.h"
#include "Bang/SerializableObject.h"

Inspector::Inspector(QWidget *parent)
    : DragDropQListWidget(), m_iContextMenu(this)
{
    setAlternatingRowColors(true);
    setSelectionMode(QAbstractItemView::SingleSelection);

    m_titleLabel      = parent->findChild<QLabel*>("labelInspectorName");
    m_enabledCheckBox = parent->findChild<QCheckBox*>("enabledInspectorCheckBox");
    m_enabledCheckBox->setVisible(false);
    setMinimumWidth(300);

    QObject::connect(m_enabledCheckBox, SIGNAL(toggled(bool)),
                     this, SLOT( OnEnabledCheckBoxChanged(bool) ));

    horizontalScrollBar()->setEnabled(false);
}

void Inspector::OnWindowShown()
{
    this->SetDragDropEventPossibleSources({
          Explorer::GetInstance(), Hierarchy::GetInstance()});
    Clear();
}

void Inspector::updateGeometries()
{
    QListWidget::updateGeometries();
    verticalScrollBar()->setSingleStep(3);
}

void Inspector::Clear()
{
    ENSURE(m_currentInspectable);

    m_currentInspectable = nullptr;
    while (!m_currentInspectableChildren.Empty())
    {
        SerializableObject *serialObject = m_currentInspectableChildren.Front();
        if (serialObject->GetInspectorFlags()->IsOn(
                    SerializableObject::InspectorFlag::DeleteWhenCleared))
        {
            delete serialObject; // This will remove it from the list
        }
        else
        {
            m_currentInspectableChildren.Remove(serialObject);
        }
    }
    for (InspectorWidget *iw : m_currentInspectorWidgets) { delete iw; }

    m_currentInspectorWidgets.Clear();
    m_widget_To_Inspectables.Clear();
    m_currentInspectableChildren.Clear();
    m_widget_To_Item.Clear();

    m_titleLabel->setText(tr(""));
    m_enabledCheckBox->setVisible(false);

    clear();
}

void Inspector::Refresh()
{
    if (!m_currentInspectableChildren.Empty())
    {
        SerializableObject *insp = m_currentInspectableChildren.Front();
        ShowInspectable(insp);
    }
    else { Clear(); }

    RefreshSizeHints();
}

void Inspector::ShowInspectable(SerializableObject *insp, const String &title)
{
    Clear();

    ENSURE(insp);
    m_currentInspectable = insp;

    typedef SerializableObject::InspectorFlag InspFlag;
    bool showEnabledCheckBox = insp->GetInspectorFlags()->IsOn(
                                          InspFlag::IsEnabledCheckBoxVisible);
    m_enabledCheckBox->setVisible(showEnabledCheckBox);

    bool checkBoxEnabled = insp->GetInspectorFlags()->IsOn(InspFlag::IsEnabled);
    m_enabledCheckBox->setChecked(checkBoxEnabled);

    List<SerializableObject*> serialObjects =
            insp->GetInspectorSerializableObjects();

    m_currentInspectable = insp;
    for (SerializableObject *serialObject : serialObjects)
    {
        InspectorWidget *iw = serialObject->GetNewInspectorWidget();
        if (iw)
        {
            m_widget_To_Inspectables[iw] = serialObject;
            m_currentInspectableChildren.PushBack(serialObject);

            iw->RefreshWidgetValues();
            AddWidget(iw);
        }
    }

    if (title.Empty())
    {
        String inspTitle = m_currentInspectable->GetTitleInInspector();
        m_titleLabel->setText(inspTitle.ToQString());
    }
    else { m_titleLabel->setText(title.ToQString()); }

    RefreshSizeHints();
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

void Inspector::OnEnabledCheckBoxChanged(bool checked)
{
    ENSURE(m_currentInspectable);
    m_currentInspectable->OnEnabledChanged(checked);
}

SerializableObject *Inspector::GetCurrentInspectable()
{
    return m_currentInspectable;
}

void Inspector::OnSerializableObjectDestroyed(SerializableObject *destroyed)
{
    bool mustRefresh = (destroyed == m_currentInspectable);

    if (m_currentInspectableChildren.Contains(destroyed))
    {
        m_currentInspectableChildren.Remove(destroyed);
        mustRefresh = true;
    }

    List<InspectorWidget*> widgetsToRemove =
            m_widget_To_Inspectables.GetKeysWithValue(destroyed);
    for (InspectorWidget *inspectorWidget : widgetsToRemove)
    {
        QListWidgetItem *item = m_widget_To_Item[inspectorWidget];
        removeItemWidget(item);
        m_widget_To_Inspectables.Remove(inspectorWidget);
        m_widget_To_Item.Remove(inspectorWidget);
        m_currentInspectorWidgets.Remove(inspectorWidget);
        delete inspectorWidget;
    }

    if (mustRefresh)
    {
        Refresh();
    }
}

void Inspector::AddWidget(InspectorWidget *widget, int row)
{
    ENSURE(widget);

    int newRow = (row == -1 ? count() : row);

    QListWidgetItem *item = new QListWidgetItem();
    insertItem(newRow, item);

    m_widget_To_Item[widget] = item;
    m_currentInspectorWidgets.PushBack(widget);

    setItemWidget(item, widget);
}

bool Inspector::IsShowingInspectable(SerializableObject *inspectable) const
{
    return m_currentInspectableChildren.Contains(inspectable);
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
        bool nextOneIsCap = (it != labelFormatted.End()) &&
                             String::IsUpperCase(*it);
        --it;
        if (!nextOneIsCap && String::IsUpperCase(*it) &&
            it != labelFormatted.Begin())
        {
            it = labelFormatted.insert(it, ' ');
            ++it;
        }
    }

    labelFormatted = labelFormatted;
    return labelFormatted.Replace("  ", " ");
}



