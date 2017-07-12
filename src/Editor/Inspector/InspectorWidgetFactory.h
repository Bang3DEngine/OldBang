#ifndef INSPECTORWIDGETFACTORY_H
#define INSPECTORWIDGETFACTORY_H

#include "Bang/BFile.h"

class Component;
class AudioSource;
class IInspectable;
class InspectorWidget;
class InspectorWidgetFactory
{
public:
    static InspectorWidget* CreateWidget(IInspectable *inspectable);
    static InspectorWidget* CreateWidget(Component *comp);
    static InspectorWidget* CreateWidget(const BFile &file);

private:
    InspectorWidgetFactory() = delete;
};

#endif // INSPECTORWIDGETFACTORY_H
