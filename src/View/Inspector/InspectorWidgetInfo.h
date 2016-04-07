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

class InspectorWidgetInfoSlotVecFloat : public InspectorPartSlotInfo
{
public:
    std::vector<float> value;

    InspectorWidgetInfoSlotVecFloat(const std::string &label,
                                  const std::vector<float> &initialValues) : InspectorPartSlotInfo(label)
    {
        this->value = initialValues;
    }
    virtual ~InspectorWidgetInfoSlotVecFloat() {}

    bool IsOfTypeVecFloat() override { return true; }
};


class InspectorWidgetInfoSlotEnum : public InspectorPartSlotInfo
{
public:
    std::vector<std::string> enumValues;
    int selectedValueIndex = 0;

    InspectorWidgetInfoSlotEnum(const std::string &label,
                              const std::vector<std::string> &initialValues) : InspectorPartSlotInfo(label)
    {
        this->enumValues = initialValues;
    }
    virtual ~InspectorWidgetInfoSlotEnum() {}

    bool IsOfTypeVecFloat() override { return false; }
};

class InspectorWidgetInfoSlotAsset: public InspectorPartSlotInfo
{
public:
    std::string filepath = "";
    std::string fileExtension = "*";

    InspectorWidgetInfoSlotAsset(const std::string& label, const std::string& fileExtension) : InspectorPartSlotInfo(label)
    {
        this->fileExtension = fileExtension;
    }

    virtual ~InspectorWidgetInfoSlotAsset() {}

    bool IsOfTypeVecFloat() override { return false; }
};



class InspectorWidgetInfo
{
private:
    std::vector<InspectorPartSlotInfo*> slotInfos;

public:

    InspectorWidgetInfo();
    virtual ~InspectorWidgetInfo() {}

    InspectorPartSlotInfo* GetSlotInfo(int i) const
    {
        return slotInfos[i];
    }

    const std::vector<InspectorPartSlotInfo*>& GetSlotInfos() const
    {
        return slotInfos;
    }

    void SetSlotsInfos(const std::vector<InspectorPartSlotInfo*> slotInfos)
    {
        this->slotInfos = slotInfos;
    }

    void AddSlotInfo(InspectorPartSlotInfo* slotInfo)
    {
        slotInfos.push_back(slotInfo);
    }
};

#endif // LISTINSPECTORITEMINFO_H
