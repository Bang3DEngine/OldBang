#ifndef LISTINSPECTORITEMINFO_H
#define LISTINSPECTORITEMINFO_H

#include <vector>
#include <string>

#include "Logger.h"

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


class InspectorPartInfoSlotEnum : public InspectorPartSlotInfo
{
public:
    std::vector<std::string> enumValues;
    int selectedValueIndex = 0;

    InspectorPartInfoSlotEnum(const std::string &label,
                              const std::vector<std::string> &initialValues) : InspectorPartSlotInfo(label)
    {
        this->enumValues = initialValues;
    }
    virtual ~InspectorPartInfoSlotEnum() {}

    bool IsOfTypeVecFloat() override { return false; }
};

class InspectorPartInfoSlotAsset: public InspectorPartSlotInfo
{
public:
    std::string filepath = "";
    std::string fileExtension = "*";

    InspectorPartInfoSlotAsset(const std::string& label, const std::string& fileExtension) : InspectorPartSlotInfo(label)
    {
        this->fileExtension = fileExtension;
    }

    virtual ~InspectorPartInfoSlotAsset() {}

    bool IsOfTypeVecFloat() override { return false; }
};





class InspectorPartInfo
{
public:
    std::vector<InspectorPartSlotInfo*> slotInfos;

    InspectorPartInfo();
    virtual ~InspectorPartInfo() {}
};

#endif // LISTINSPECTORITEMINFO_H
