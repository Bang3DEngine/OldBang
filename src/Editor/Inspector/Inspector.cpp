#include "Bang/Inspector.h"

#include <QLabel>
#include <QDropEvent>
#include <QScrollBar>
#include "Bang/WinUndef.h"

#include "Bang/BFile.h"
#include "Bang/Component.h"
#include "Bang/GameObject.h"
#include "Bang/EditorWindow.h"
#include "Bang/InspectorWidget.h"
#include "Bang/SerializableObject.h"
#include "Bang/InspectorWidgetFactory.h"

Inspector::Inspector(QWidget *parent) : DragDropQListWidget(),
                                        m_iContextMenu(this)
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
    qRegisterMetaType<IAttributeWidget>("IAttributeWidget");
    Clear();
}

void Inspector::Show(Object *object, bool tmpObject)
{
    String title = "";
    if (object->IsOfType<GameObject>())
    {
        GameObject *go = static_cast<GameObject*>(object);
        Show( InspectorWidgetFactory::CreateWidgets(go) );
        title = go->GetName();
    }
    else
    {
        Show( { InspectorWidgetFactory::CreateWidget(object) } );
        if (object->IsOfType<BFile>())
        {
            const BFile *f = static_cast<const BFile*>(object);
            title = f->GetPath().GetName();
        }
    }
    m_titleLabel->setText(title.ToQString());

    p_inspectedObject = !tmpObject ? object : nullptr;
    if (p_inspectedObject) { p_inspectedObject->RegisterDestroyListener(this); }
}

Object *Inspector::GetInspectedObject() const
{
    return p_inspectedObject;
}

void Inspector::Clear()
{
    for (InspectorWidget *inspWidget : GetCurrentInspectorWidgets())
    {
        inspWidget->OnDestroy();
    }

    p_inspectedObject = nullptr;
    m_currentInspectorWidgets.Clear();

    m_titleLabel->setText(tr(""));
    m_enabledCheckBox->setVisible(false);

    RefreshSizeHints();

    clear();
}

bool Inspector::Refresh()
{
    return true;
}

void Inspector::Show(const List<InspectorWidget *> &inspWidgets)
{
    Clear();

    for (InspectorWidget *inspWidget : inspWidgets)
    {
        InsertInspectorWidget(inspWidget);
    }
    RefreshSizeHints();
}

void Inspector::RefreshSizeHints()
{
    for (int i = 0; i < count(); ++i)
    {
        QListWidgetItem *itm = item(i);
        InspectorWidget *iw = Object::SCast<InspectorWidget>(itemWidget(itm));
        if (iw)
        {
            itm->setSizeHint( QSize(iw->size().width(),
                                    iw->GetHeightSizeHint()) );
        }
    }
}

void Inspector::OnDestroyableDestroyed(Destroyable *destroyedObject)
{
    if ( destroyedObject == p_inspectedObject )
    {
        // The object being inspected has been destroyed
        Clear();
    }
}

void Inspector::OnDestroyDemanded(Destroyable *objectDemandingDestroy)
{
    // Delete the widget corresponding to the destroyed object
    for (InspectorWidget *inspWidget : GetCurrentInspectorWidgets())
    {
        if (objectDemandingDestroy == inspWidget)
        {
            for (int i = 0; i < count(); ++i)
            {
                QListWidgetItem *itm = item(i);
                if (itemWidget(itm) == inspWidget)
                {
                    m_currentInspectorWidgets.Remove(inspWidget);
                    inspWidget->OnDestroy();
                    delete itm;
                }
            }
        }
    }
}

void Inspector::OnEnabledCheckBoxChanged(bool checked)
{
    if (Object::IsOfType<GameObject>(p_inspectedObject))
    {
        GameObject *go = Object::SCast<GameObject>(p_inspectedObject);
        go->SetEnabled(checked);
    }
}

void Inspector::InsertInspectorWidget(InspectorWidget *inspWidget, int row)
{
    ENSURE(inspWidget);

    int newRow = (row == -1 ? count() : row);

    QListWidgetItem *item = new QListWidgetItem();
    insertItem(newRow, item);
    setItemWidget(item, inspWidget);

    inspWidget->RegisterDestroyListener(this);
    m_currentInspectorWidgets.PushBack(inspWidget);
}

Inspector *Inspector::GetInstance()
{
    EditorWindow *ew = EditorWindow::GetInstance();
    return ew ? ew->widgetInspector : nullptr;
}

const List<InspectorWidget *> Inspector::GetCurrentInspectorWidgets() const
{
    return m_currentInspectorWidgets;
}

void Inspector::dropEvent(QDropEvent *e) { e->ignore(); }

void Inspector::updateGeometries()
{
    QListWidget::updateGeometries();
    verticalScrollBar()->setSingleStep(3);
}
