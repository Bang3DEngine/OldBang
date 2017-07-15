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
#include "Bang/InspectorWidgetGroup.h"
#include "Bang/InspectorWidgetFactory.h"

Inspector::Inspector(QWidget *parent) : DragDropQListWidget(),
                                        m_iContextMenu(this)
{
    setAlternatingRowColors(true);
    setSelectionMode(QAbstractItemView::SingleSelection);

    m_refreshTimer.start(100);
    QObject::connect(&m_refreshTimer, SIGNAL(timeout()), this, SLOT(OnUpdate()));

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
    qRegisterMetaType<IInspectorWidget>("IInspectorWidget");
    qRegisterMetaType<IAttributeWidget>("IAttributeWidget");
    Clear();
}

void Inspector::Show(Object *object, bool tmpObject)
{
    InspectorWidgetGroup* inspectorWidgetGroup =
                            InspectorWidgetFactory::CreateWidgetGroup(object);
    Show(inspectorWidgetGroup);

    p_inspectedObject = !tmpObject ? object : nullptr;
    if (p_inspectedObject) { p_inspectedObject->RegisterDestroyListener(this); }
}

Object *Inspector::GetInspectedObject() const
{
    return p_inspectedObject;
}

const List<InspectorWidget*>& Inspector::GetCurrentInspectorWidgets() const
{
    if (m_currentInspectorWidgetGroup)
    {
        return m_currentInspectorWidgetGroup->GetInspectorWidgets();
    }

    static List<InspectorWidget*> empty;
    return empty;
}

void Inspector::Clear()
{
    for (InspectorWidget *inspWidget : GetCurrentInspectorWidgets())
    {
        inspWidget->OnDestroy();
    }

    if (m_currentInspectorWidgetGroup)
    {
        delete m_currentInspectorWidgetGroup;
        m_currentInspectorWidgetGroup = nullptr;
    }

    p_inspectedObject = nullptr;
    m_titleLabel->setText(tr(""));
    m_enabledCheckBox->setVisible(false);

    RefreshSizeHints();

    clear();
}

void Inspector::OnUpdate()
{
    if (m_currentInspectorWidgetGroup)
    {
        m_currentInspectorWidgetGroup->OnUpdate();
    }
}

void Inspector::Show(InspectorWidgetGroup* inspWidgetGroup)
{
    Clear();

    m_currentInspectorWidgetGroup = inspWidgetGroup;
    for (InspectorWidget *inspWidget : inspWidgetGroup->GetInspectorWidgets())
    {
        InsertInspectorWidget(inspWidget);
    }

    m_enabledCheckBox->setChecked( inspWidgetGroup->IsEnabled() );
    m_enabledCheckBox->setVisible( inspWidgetGroup->NeedsEnableCheckBox() );
    m_titleLabel->setText( inspWidgetGroup->GetTitle().ToQString() );

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
        Clear(); // The object being inspected has been destroyed
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
                    inspWidget->OnDestroy();
                    delete itm;
                }
            }
        }
    }
}

void Inspector::OnEnabledCheckBoxChanged(bool checked)
{
    if (m_currentInspectorWidgetGroup)
    {
        m_currentInspectorWidgetGroup->OnEnableCheckBoxChanged(checked);
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
}

Inspector *Inspector::GetInstance()
{
    EditorWindow *ew = EditorWindow::GetInstance();
    return ew ? ew->widgetInspector : nullptr;
}

const InspectorWidgetGroup* Inspector::GetCurrentInspectorWidgetGroup() const
{
    return m_currentInspectorWidgetGroup;
}

void Inspector::dropEvent(QDropEvent *e) { e->ignore(); }

void Inspector::updateGeometries()
{
    QListWidget::updateGeometries();
    verticalScrollBar()->setSingleStep(3);
}
