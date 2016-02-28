#ifndef LISTINSPECTORITEMINFO_H
#define LISTINSPECTORITEMINFO_H

#include <vector>
#include <string>

class InspectorPartSlotInfo
{
protected:
    InspectorPartSlotInfo(const std::string &label) : label(label){ }

public:
    std::string label;
    virtual bool IsOfTypeVecFloat() = 0;

    virtual ~InspectorPartSlotInfo() {}
};



class InspectorPartInfoSlotVecFloat : public InspectorPartSlotInfo
{
public:
    std::vector<float> value;

    InspectorPartInfoSlotVecFloat(const std::string &label,
                                      const std::vector<float> &initialValues) : InspectorPartSlotInfo(label)
    {
        this->value = initialValues;
    }
    virtual ~InspectorPartInfoSlotVecFloat() {}

    bool IsOfTypeVecFloat() override { return true; }
};



class InspectorPartInfo
{
public:
    std::vector<InspectorPartSlotInfo*> slotInfos;

    InspectorPartInfo();
    virtual ~InspectorPartInfo() {}
};

#endif // LISTINSPECTORITEMINFO_H
