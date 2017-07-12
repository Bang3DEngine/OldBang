#ifndef INSPECTORWIDGETFACTORY_H
#define INSPECTORWIDGETFACTORY_H

#include "Bang/List.h"
#include "Bang/BFile.h"

class Component;
class GameObject;
class AudioSource;
class InspectorWidget;
class InspectorWidgetFactory
{
public:
    static List<InspectorWidget*> CreateWidgets(GameObject *go);
    static InspectorWidget* CreateWidget(Component *comp);
    static InspectorWidget* CreateWidget(const BFile &file);

private:
    InspectorWidgetFactory() = delete;
};

#endif // INSPECTORWIDGETFACTORY_H
