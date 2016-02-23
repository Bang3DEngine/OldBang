#include "ListInspector.h"

#include "Entity.h"
#include "Part.h"
#include "Logger.h"

ListInspector::ListInspector(QWidget *parent)
{
}

void ListInspector::OnTreeHierarchyEntitySelected(Entity *selectedEntity)
{
    QLabel *nameLabel = parent()->findChild<QLabel*>("labelInspectorEntityName");
    if(nameLabel != nullptr)
    {
        nameLabel->setText(QString::fromStdString("Name: " + selectedEntity->GetName()));
    }

    clear();
    for(Part *p : *(selectedEntity->GetParts()))
    {
        addItem(QString::fromStdString(p->ToString()));
    }
}
