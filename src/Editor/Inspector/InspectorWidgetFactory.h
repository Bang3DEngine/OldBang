#ifndef INSPECTORWIDGETFACTORY_H
#define INSPECTORWIDGETFACTORY_H

class Component;
class AudioSource;
class IInspectable;
class InspectorWidget;
class InspectorWidgetFactory
{
public:
    static InspectorWidget* CreateWidget(IInspectable *inspectable);
    static InspectorWidget* CreateWidget(Component *comp);

private:
    InspectorWidgetFactory() = delete;
};

#endif // INSPECTORWIDGETFACTORY_H
