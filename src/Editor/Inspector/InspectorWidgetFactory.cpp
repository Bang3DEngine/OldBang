#include "Bang/InspectorWidgetFactory.h"

#include "Bang/Debug.h"
#include "Bang/Component.h"
#include "Bang/AudioSource.h"
#include "Bang/ComponentWidget.h"
#include "Bang/AudioSourceInspectorWidget.h"

InspectorWidget *InspectorWidgetFactory::CreateWidget(Component *component)
{
    InspectorWidget *inspWidget = nullptr;

    // N-ary dispatch...using RTTI
    if (component->IsOfType<AudioSource>())
    {
        inspWidget = _CreateWidget( static_cast<AudioSource*>(component) );
    }
    else
    {
        inspWidget = _CreateWidget(component);
    }

    return inspWidget;
}

InspectorWidget *InspectorWidgetFactory::_CreateWidget(Component *component)
{
    return new ComponentWidget(component);
}

InspectorWidget *InspectorWidgetFactory::_CreateWidget(AudioSource *audioSource)
{
    return new AudioSourceInspectorWidget(audioSource);
}
