#ifndef ATTRWIDGETBUTTONLISTENER_H
#define ATTRWIDGETBUTTONLISTENER_H

#include "String.h"

class IAttrWidgetButtonListener
{
protected:
    IAttrWidgetButtonListener();

public:
    virtual void OnButtonClicked(const String &attrName) = 0;
};

#endif // ATTRWIDGETBUTTONLISTENER_H
