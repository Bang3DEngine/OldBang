#include "Bang/InspectorWidgetFactory.h"

#include "Bang/Debug.h"
#include "Bang/Component.h"
#include "Bang/AudioSource.h"
#include "Bang/ComponentWidget.h"
#include "Bang/AudioSourceInspectorWidget.h"

InspectorWidget *InspectorWidgetFactory::CreateWidget(IInspectable *inspectable)
{
    InspectorWidget *inspWidget = new InspectorWidget();
    inspWidget->Init(inspectable);
    return inspWidget;
}

InspectorWidget *InspectorWidgetFactory::CreateWidget(Component *component)
{
    ComponentWidget *compWidget = nullptr;

    // N-ary dispatch...using RTTI
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
