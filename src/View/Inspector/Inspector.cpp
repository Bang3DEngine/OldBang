#include "Inspector.h"

#include "Entity.h"
#include "Part.h"
#include "Behaviour.h"
#include "Camera.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Logger.h"

Inspector::Inspector(QWidget *parent) : QListWidget(parent)
{
}

void Inspector::Refresh()
{
    QLabel *nameLabel = parent()->findChild<QLabel*>("labelInspectorEntityName");
    if(currentEntity != nullptr)
    {
        if(nameLabel != nullptr)
        {
            nameLabel->setText(QString::fromStdString("Name: " + currentEntity->GetName()));
        }

        clear();

        //For every part, create a widgetItem by reading its InspectorWidgetInfo
        for(Part *p : currentEntity->GetParts())
        {
            QListWidgetItem *item = new QListWidgetItem();
            addItem(item);

            InspectorPartWidget *iw = new InspectorPartWidget(p);

            this->setItemWidget(item, iw);
            item->setSizeHint(iw->size());

            this->adjustSize();
            this->show();
        }
    }
    else
    {
        clear();
        if(nameLabel != nullptr)
        {
            nameLabel->setText(QString::fromStdString("No entity selected."));
        }
    }

    this->setStyleSheet("/* */"); //without this line we get resize problems :)
    this->show();
}

void Inspector::OnTreeHierarchyEntitiesSelected(const std::list<Entity*> &selectedEntities)
{
    if(!selectedEntities.empty())
    {
        currentEntity = selectedEntities.front();
    }
    else currentEntity = nullptr;

    Refresh();
}

void Inspector::OnMenuBarActionClicked(MenuBar::Action clickedAction)
{
    if(clickedAction == MenuBar::Action::AddPartBehaviour)
    {
        if(this->currentEntity != nullptr)
        {
            Behaviour *b = new Behaviour();
            currentEntity->AddPart(b);
        }
    }
    else if(clickedAction == MenuBar::Action::AddPartCamera)
    {
        if(this->currentEntity != nullptr)
        {
            Camera *c = new Camera();
            currentEntity->AddPart(c);
        }
    }
    else if(clickedAction == MenuBar::Action::AddPartMeshRenderer)
    {
        if(this->currentEntity != nullptr)
        {
            MeshRenderer *m = new MeshRenderer();
            currentEntity->AddPart(m);
        }
    }
    else if(clickedAction == MenuBar::Action::AddPartTransform)
    {
        if(this->currentEntity != nullptr)
        {
            Transform *t = new Transform();
            currentEntity->AddPart(t);
        }
    }

    Refresh();
}


