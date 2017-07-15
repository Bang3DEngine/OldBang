#ifndef INSPECTORWIDGETFACTORY_H
#define INSPECTORWIDGETFACTORY_H

#include "Bang/List.h"
#include "Bang/BFile.h"

class Component;
class GameObject;
class AudioSource;
class InspectorWidget;
class InspectorWidgetGroup;
class InspectorWidgetFactory
{
public:
    static InspectorWidgetGroup* CreateWidgetGroup(Object *obj);
    static InspectorWidget* CreateComponentWidget(Component *comp);

private:
    InspectorWidgetFactory() = delete;
};

#endif // INSPECTORWIDGETFACTORY_H
