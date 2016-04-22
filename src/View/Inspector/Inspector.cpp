#include "Inspector.h"

#include "Entity.h"
#include "Part.h"
#include "Behaviour.h"
#include "Camera.h"
#include "MeshRenderer.h"
#include "LineRenderer.h"
#include "Transform.h"
#include "Logger.h"

Inspector::Inspector(QWidget *parent) : QListWidget(parent)
{
    titleLabel = parent->findChild<QLabel*>("labelInspectorEntityName");
}

void Inspector::Clear()
{
    clear();
    currentEntity = nullptr;
    titleLabel->setText(QString::fromStdString("No entity selected."));

    setStyleSheet("/* */"); //without this line we get resize problems :)
    show();
}

void Inspector::Refresh()
{
    Entity *e = currentEntity;
    Clear();
    ShowEntityInfo(e);
}

void Inspector::ShowEntityInfo(Entity *entity)
{
    Clear();
    currentEntity = entity;

    if(currentEntity == nullptr) return;

    for(Part *p : currentEntity->GetParts())
    {
        InspectorPartWidget *w = new InspectorPartWidget(p);
        AddWidget(w);
    }

    titleLabel->setText(
                QString::fromStdString("Name: " + currentEntity->GetName())
                );
}

void Inspector::SetWidget(InspectorWidget *widget)
{
    Clear();
    AddWidget(widget);
}

void Inspector::AddWidget(InspectorWidget *widget)
{
    QListWidgetItem *item = new QListWidgetItem();
    addItem(item);

    setItemWidget(item, widget);
    item->setSizeHint(widget->size());

    adjustSize();
    setStyleSheet("/* */"); //without this line we get resize problems :)
    show();
}

void Inspector::OnTreeHierarchyEntitiesSelected
    (const std::list<Entity*> &selectedEntities)
{
    Entity *e = nullptr;
    if(!selectedEntities.empty())
    {
        e = selectedEntities.front();
    }

    ShowEntityInfo(e);
}

void Inspector::OnMenuBarActionClicked(MenuBar::Action clickedAction)
{
    if(currentEntity == nullptr) return;

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
    else if(clickedAction == MenuBar::Action::AddPartLineRenderer)
    {
        if(this->currentEntity != nullptr)
        {
            LineRenderer *lr = new LineRenderer();
            currentEntity->AddPart(lr);
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


