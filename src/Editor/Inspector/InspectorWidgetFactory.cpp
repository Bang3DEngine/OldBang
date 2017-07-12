#include "Bang/InspectorWidgetFactory.h"

#include "Bang/Debug.h"
#include "Bang/Component.h"
#include "Bang/GameObject.h"
#include "Bang/AudioSource.h"
#include "Bang/AudioClipFile.h"
#include "Bang/ComponentWidget.h"
#include "Bang/AudioSourceInspectorWidget.h"
#include "Bang/AudioClipFileInspectorWidget.h"

List<InspectorWidget*> InspectorWidgetFactory::CreateWidgets(GameObject *go)
{
    List<InspectorWidget*> componentWidgets;
    for (Component *comp : go->GetComponents())
    {
        InspectorWidget *compIW = InspectorWidgetFactory::CreateWidget(comp);
        componentWidgets.PushBack(compIW);
    }
    return componentWidgets;
}

InspectorWidget* InspectorWidgetFactory::CreateWidget(Component *component)
{
    ComponentWidget *compWidget = nullptr;
    if (component->IsOfType<AudioSource>())
    {
        AudioSource *as = static_cast<AudioSource*>(component);
        compWidget = new AudioSourceInspectorWidget(as);
    }
    else
    {
        compWidget = new ComponentWidget(component);
    }

    compWidget->Init();
    compWidget->Refresh();
    return compWidget;
}

InspectorWidget* InspectorWidgetFactory::CreateWidget(const BFile &file)
{
    Debug_Log("Create widget for " << file.GetPath());

    InspectorWidget *inspWidget = nullptr;
    if ( Object::IsOfType<AudioClipFile>(&file) )
    {
        const AudioClipFile &audioFile = static_cast<const AudioClipFile&>(file);
        inspWidget = new AudioClipFileInspectorWidget(audioFile);
    }
    else
    {
        inspWidget = new InspectorWidget();
    }

    inspWidget->Init();
    inspWidget->Refresh();
    return inspWidget;
}
