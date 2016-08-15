#ifndef ATTRWIDGETBUTTONLISTENER_H
#define ATTRWIDGETBUTTONLISTENER_H

#include <string>

class IAttrWidgetButtonListener
{
protected:
    IAttrWidgetButtonListener();

public:
    virtual void OnButtonClicked(const std::string &attrName) = 0;
};

#endif // ATTRWIDGETBUTTONLISTENER_H
