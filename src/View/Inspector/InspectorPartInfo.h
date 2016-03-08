#ifndef LISTINSPECTORITEMINFO_H
#define LISTINSPECTORITEMINFO_H

#include <vector>
#include <string>

class InspectorPartSlotInfoBase
{
protected:
    InspectorPartSlotInfoBase() {}
    virtual ~InspectorPartSlotInfoBase() {}
};

template<class T>
class InspectorPartSlotInfo : public InspectorPartSlotInfoBase
{
protected:
    std::string label;
    T value;
    int selectedIndex; //useful for enums, for example

public:
    InspectorPartSlotInfo(const std::string &label,
                          const T &value) : label(label), value(value) {}

    virtual ~InspectorPartSlotInfo() {}

    std::string GetLabel() { return label; }
    void SetValue(const T &value) { this->value = value; }
    T GetValue() { return value; }
    int GetSelectedIndex() { return selectedIndex; }

    int SetSelectedIndex(int index) { selectedIndex = index; }
};

class InspectorPartInfo
{
public:
    std::vector<InspectorPartSlotInfoBase*> slotInfos;
};

#endif // LISTINSPECTORITEMINFO_H
