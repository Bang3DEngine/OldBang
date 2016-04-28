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
                        const std::vector<std::string> &enumValues,
                        int initialSelectedIndex = 0) : InspectorSWInfo(label)
    {
        this->enumValues = enumValues;
        this->selectedValueIndex = initialSelectedIndex;
    }
    virtual ~InspectorEnumSWInfo() {}

    bool IsOfTypeVecFloat() override { return false; }
};

class InspectorStringSWInfo: public InspectorSWInfo
{
public:

    bool readonly = false, inlined = false;
    std::string value = "";

    InspectorStringSWInfo(const std::string& label, const std::string& initialValue,
                          bool readonly = false, bool inlined = false) : InspectorSWInfo(label)
    {
        this->value = initialValue;
        this->readonly = readonly;
        this->inlined = inlined;
    }
    virtual ~InspectorStringSWInfo() {}

    bool IsOfTypeVecFloat() override { return false; }
};


class InspectorFileSWInfo: public InspectorSWInfo
{
public:
    std::string filepath = "";
    std::string fileExtension = "*";

    InspectorFileSWInfo(const std::string& label, const std::string& fileExtension) : InspectorSWInfo(label)
    {
        this->fileExtension = fileExtension;
    }

    virtual ~InspectorFileSWInfo() {}

    bool IsOfTypeVecFloat() override { return false; }
};

//Contains all SWInfos
class InspectorWidgetInfo
{
private:
    std::vector<InspectorSWInfo*> slotInfos;

public:

    InspectorWidgetInfo() {}
    virtual ~InspectorWidgetInfo()
    {
        for(int i = 0; i < slotInfos.size(); ++i) delete slotInfos[i];
    }

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
