#include "Bang/InspectorWidgetFactory.h"

#include "Bang/Debug.h"
#include "Bang/Camera.h"
#include "Bang/Behaviour.h"
#include "Bang/Component.h"
#include "Bang/GameObject.h"
#include "Bang/AudioSource.h"
#include "Bang/AudioListener.h"
#include "Bang/AudioClipFile.h"
#include "Bang/LineRenderer.h"
#include "Bang/CircleRenderer.h"
#include "Bang/MeshRenderer.h"
#include "Bang/ComponentWidget.h"
#include "Bang/CameraInspectorWidget.h"
#include "Bang/BehaviourInspectorWidget.h"
#include "Bang/AudioSourceInspectorWidget.h"
#include "Bang/MeshRendererInspectorWidget.h"
#include "Bang/LineRendererInspectorWidget.h"
#include "Bang/AudioListenerInspectorWidget.h"
#include "Bang/AudioClipFileInspectorWidget.h"
#include "Bang/CircleRendererInspectorWidget.h"

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
    else if (component->IsOfType<AudioListener>())
    {
        AudioListener *al = static_cast<AudioListener*>(component);
        compWidget = new AudioListenerInspectorWidget(al);
    }
    else if (component->IsOfType<Behaviour>())
    {
        Behaviour *b = static_cast<Behaviour*>(component);
        compWidget = new BehaviourInspectorWidget(b);
    }
    else if (component->IsOfType<Camera>())
    {
        Camera *c = static_cast<Camera*>(component);
        compWidget = new CameraInspectorWidget(c);
    }
    else if (component->IsOfType<CircleRenderer>())
    {
        CircleRenderer *cr = static_cast<CircleRenderer*>(component);
        compWidget = new CircleRendererInspectorWidget(cr);
    }
    else if (component->IsOfType<LineRenderer>())
    {
        LineRenderer *lr = static_cast<LineRenderer*>(component);
        compWidget = new LineRendererInspectorWidget(lr);
    }
    else if (component->IsOfType<MeshRenderer>())
    {
        MeshRenderer *mr = static_cast<MeshRenderer*>(component);
        compWidget = new MeshRendererInspectorWidget(mr);
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
