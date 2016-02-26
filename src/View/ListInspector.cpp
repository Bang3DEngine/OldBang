#include "ListInspector.h"

#include "Entity.h"
#include "Part.h"
#include "Logger.h"

ListInspector::ListInspector(QWidget *parent) : QListWidget(parent)
{
}

void ListInspector::OnTreeHierarchyEntitiesSelected(const std::list<Entity*> &selectedEntities)
{
    if(!selectedEntities.empty())
    {
        Entity *selectedEntity = selectedEntities.front();

        QLabel *nameLabel = parent()->findChild<QLabel*>("labelInspectorEntityName");
        if(nameLabel != nullptr)
        {
            nameLabel->setText(QString::fromStdString("Name: " + selectedEntity->GetName()));
        }

        clear();

        //For every part, create a widgetItem by reading its ListInspectorItemInfo
        for(Part *p : selectedEntity->GetParts())
        {
            QListWidgetItem *item = new QListWidgetItem();
            addItem(item);

            ListInspectorItemWidget *iw = new ListInspectorItemWidget(p->GetName(),
                                                                      p->inspectorItemInfo);
            this->setItemWidget(item, iw);
            item->setSizeHint(iw->size() * 0.9);

            this->adjustSize();
            this->show();
        }
    }
}
