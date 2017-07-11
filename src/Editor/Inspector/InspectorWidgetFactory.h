#ifndef INSPECTORWIDGETFACTORY_H
#define INSPECTORWIDGETFACTORY_H

class Component;
class AudioSource;
class InspectorWidget;
class InspectorWidgetFactory
{
public:
    static InspectorWidget* CreateWidget(Component *component);

private:
    static InspectorWidget* _CreateWidget(Component *component);
    static InspectorWidget* _CreateWidget(AudioSource *audioSource);

    InspectorWidgetFactory() = delete;
};

#endif // INSPECTORWIDGETFACTORY_H
