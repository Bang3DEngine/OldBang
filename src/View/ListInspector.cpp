#include "ListInspector.h"

#include "Entity.h"
#include "Logger.h"

ListInspector::ListInspector(QWidget *parent)
{
    addItem("jkasdhkjasdhjslda");
}

void ListInspector::OnTreeHierarchyEntitySelected(Entity *selectedEntity)
{
    addItem(QString::fromStdString(selectedEntity->GetName()));
    Logger_Log(selectedEntity->GetName());
}
