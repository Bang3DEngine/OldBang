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

    m_updateTimer.start(100);
    QObject::connect(&m_updateTimer, SIGNAL(timeout()), this, SLOT(OnUpdate()));

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
    if (m_currentInspectorWidgetGroup)
    {
        delete m_currentInspectorWidgetGroup;
        m_currentInspectorWidgetGroup = nullptr;
    }
    p_inspectedObject = nullptr;

    m_titleLabel->setText(tr(""));
    m_enabledCheckBox->setVisible(false);

    UpdateSizeHints();
    clear();
}

void Inspector::OnUpdate()
{
    if (m_currentInspectorWidgetGroup)
    {
        m_currentInspectorWidgetGroup->OnUpdate();
    }
    UpdateSizeHints();
}

void Inspector::Show(InspectorWidgetGroup* inspWidgetGroup)
{
    Clear();

    m_currentInspectorWidgetGroup = inspWidgetGroup;

    QObject::connect(inspWidgetGroup, SIGNAL(RequestInsert(InspectorWidget*)),
                     this, SLOT(InsertInspectorWidget(InspectorWidget*)));
    QObject::connect(inspWidgetGroup, SIGNAL(RequestMove(InspectorWidget*,int)),
                     this, SLOT(MoveInspectorWidget(InspectorWidget*,int)));
    QObject::connect(inspWidgetGroup, SIGNAL(RequestRemove(InspectorWidget*)),
                     this, SLOT(RemoveInspectorWidget(InspectorWidget*)));

    m_currentInspectorWidgetGroup->Init();

    m_enabledCheckBox->setChecked( inspWidgetGroup->IsEnabled() );
    m_enabledCheckBox->setVisible( inspWidgetGroup->NeedsEnableCheckBox() );
    m_titleLabel->setText( inspWidgetGroup->GetTitle().ToQString() );

    UpdateSizeHints();
}

int Inspector::GetInspectorWidgetRow(InspectorWidget *inspectorWidget) const
{
    for (int i = 0; i < count(); ++i)
    {
        QListWidgetItem *itm = item(i);
        if (itemWidget(itm) == inspectorWidget) { return i; }
    }
    return -1;
}

void Inspector::UpdateSizeHints()
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

void Inspector::OnEnabledCheckBoxChanged(bool checked)
{
    if (m_currentInspectorWidgetGroup)
    {
        m_currentInspectorWidgetGroup->OnEnableCheckBoxChanged(checked);
    }
}

void Inspector::InsertInspectorWidget(InspectorWidget *inspWidget)
{
    ENSURE(inspWidget);

    QListWidgetItem *item = new QListWidgetItem();
    insertItem(count(), item);
    setItemWidget(item, inspWidget);
}

void Inspector::MoveInspectorWidget(InspectorWidget *inspectorWidget,
                                    int newRow)
{
    int oldRow = GetInspectorWidgetRow(inspectorWidget);
    if (oldRow >= 0)
    {
        QListWidgetItem *itemToMove = item(oldRow);
        setItemWidget(itemToMove, nullptr);
        QListWidgetItem* takenItem = takeItem(oldRow);

        Debug_Log("Insert item " << takenItem << " into " << newRow);
        insertItem(newRow, takenItem);
        setItemWidget(takenItem, inspectorWidget);
        // Debug_Log("Insert item " << itemToSwap << " into " << oldRow);
        // insertItem(0, itemToSwap);
    }
}

void Inspector::RemoveInspectorWidget(InspectorWidget *inspectorWidget)
{
    int oldRow = GetInspectorWidgetRow(inspectorWidget);
    if (oldRow >= 0)
    {
        QListWidgetItem* takenItem = takeItem(oldRow);
        delete takenItem;
    }
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
