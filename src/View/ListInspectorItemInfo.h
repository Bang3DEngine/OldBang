#ifndef LISTINSPECTORITEMINFO_H
#define LISTINSPECTORITEMINFO_H

#include <vector>
#include <string>

class ListInspectorPartSlotInfo
{
protected:
    ListInspectorPartSlotInfo(const std::string &label) : label(label){ }

public:
    std::string label;
    virtual bool IsOfTypeVecFloat() = 0;

    virtual ~ListInspectorPartSlotInfo() {}
};



class ListInspectorPartInfoSlotVecFloat : public ListInspectorPartSlotInfo
{
public:
    std::vector<float> value;

    ListInspectorPartInfoSlotVecFloat(const std::string &label,
                                      const std::vector<float> &initialValues) : ListInspectorPartSlotInfo(label)
    {
        this->value = initialValues;
    }
    virtual ~ListInspectorPartInfoSlotVecFloat() {}

    bool IsOfTypeVecFloat() override { return true; }
};



class ListInspectorPartInfo
{
public:
    std::vector<ListInspectorPartSlotInfo*> slotInfos;

    ListInspectorPartInfo();
    virtual ~ListInspectorPartInfo() {}
};

#endif // LISTINSPECTORITEMINFO_H
