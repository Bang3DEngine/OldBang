#include "Bang/InspectorWidgetFactory.h"

#include "Bang/Debug.h"
#include "Bang/Component.h"
#include "Bang/AudioSource.h"
#include "Bang/AudioClipFile.h"
#include "Bang/ComponentWidget.h"
#include "Bang/FileInspectable.h"
#include "Bang/AudioSourceInspectorWidget.h"
#include "Bang/AudioClipFileInspectorWidget.h"

InspectorWidget *InspectorWidgetFactory::CreateWidget(IInspectable *inspectable)
{
    InspectorWidget *inspWidget = new InspectorWidget();
    inspWidget->Init(inspectable);
    return inspWidget;
}

InspectorWidget *InspectorWidgetFactory::CreateWidget(Component *component)
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
    return compWidget;
}

InspectorWidget *InspectorWidgetFactory::CreateWidget(const BFile &file)
{
    Debug_Log("Create widget for " << file.GetPath());

    IInspectable *inspectable = nullptr;
    InspectorWidget *inspWidget = nullptr;
    if ( Object::IsOfType<AudioClipFile>(&file) )
    {
        Debug_Log("AudioClipFile! :)");
        const AudioClipFile &audioFile = static_cast<const AudioClipFile&>(file);
        inspWidget = new AudioClipFileInspectorWidget(audioFile);
        inspectable = new FileInspectable<AudioClipFile>(audioFile);
    }
    else
    {
        inspWidget = new InspectorWidget();
        inspectable = new FileInspectable<BFile>(file);
    }

    inspWidget->Init(inspectable);
    return inspWidget;
}
