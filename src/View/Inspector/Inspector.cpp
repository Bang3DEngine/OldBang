#include "Inspector.h"

#include "Entity.h"
#include "Part.h"
#include "Logger.h"

Inspector::Inspector(QWidget *parent) : QListWidget(parent)
{
}

void Inspector::OnTreeHierarchyEntitiesSelected(const std::list<Entity*> &selectedEntities)
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

        //For every part, create a widgetItem by reading its InspectorPartInfo
        for(Part *p : selectedEntity->GetParts())
        {
            QListWidgetItem *item = new QListWidgetItem();
            addItem(item);

            InspectorPartWidget *iw = new InspectorPartWidget(p);

            this->setItemWidget(item, iw);
            item->setSizeHint(iw->size() * 0.9);

            this->adjustSize();
            this->show();
        }
    }
}
