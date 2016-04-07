#ifndef LISTINSPECTORITEMINFO_H
#define LISTINSPECTORITEMINFO_H

#include <vector>
#include <string>

#include "Logger.h"

class InspectorSWInfo
{
protected:
    InspectorSWInfo(const std::string &label) : label(label){ }

public:
    std::string label;
    virtual bool IsOfTypeVecFloat() = 0;

    virtual ~InspectorSWInfo() {}
};

class InspectorVFloatSWInfo : public InspectorSWInfo
{
public:
    std::vector<float> value;

    InspectorVFloatSWInfo(const std::string &label,
                                  const std::vector<float> &initialValues) : InspectorSWInfo(label)
    {
        this->value = initialValues;
    }
    virtual ~InspectorVFloatSWInfo() {}

    bool IsOfTypeVecFloat() override { return true; }
};


class InspectorEnumSWInfo : public InspectorSWInfo
{
public:
    std::vector<std::string> enumValues;
    int selectedValueIndex = 0;

    InspectorEnumSWInfo(const std::string &label,
                              const std::vector<std::string> &initialValues) : InspectorSWInfo(label)
    {
        this->enumValues = initialValues;
    }
    virtual ~InspectorEnumSWInfo() {}

    bool IsOfTypeVecFloat() override { return false; }
};

class InspectorAssetSWInfo: public InspectorSWInfo
{
public:
    std::string filepath = "";
    std::string fileExtension = "*";

    InspectorAssetSWInfo(const std::string& label, const std::string& fileExtension) : InspectorSWInfo(label)
    {
        this->fileExtension = fileExtension;
    }

    virtual ~InspectorAssetSWInfo() {}

    bool IsOfTypeVecFloat() override { return false; }
};

//Contains all SWInfos
class InspectorWidgetInfo
{
private:
    std::vector<InspectorSWInfo*> slotInfos;

public:

    InspectorWidgetInfo();
    virtual ~InspectorWidgetInfo() {}

    InspectorSWInfo* GetSlotInfo(int i) const
    {
        return slotInfos[i];
    }

    const std::vector<InspectorSWInfo*>& GetSlotInfos() const
    {
        return slotInfos;
    }

    void SetSlotsInfos(const std::vector<InspectorSWInfo*> slotInfos)
    {
        this->slotInfos = slotInfos;
    }

    void AddSlotInfo(InspectorSWInfo* slotInfo)
    {
        slotInfos.push_back(slotInfo);
    }
};

#endif // LISTINSPECTORITEMINFO_H
