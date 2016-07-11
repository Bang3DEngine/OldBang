#ifndef LISTINSPECTORITEMINFO_H
#define LISTINSPECTORITEMINFO_H

#include <vector>
#include <string>
#include <functional>

#include "Logger.h"

class InspectorSWInfo
{
protected:
    InspectorSWInfo(const std::string &label) : m_label(label){ }

public:
    std::string m_label;

    /**
     * @brief Does this Component needs its label printed on top of it?
     * @return
     */
    virtual bool NeedsWrittenLabel() = 0;

    virtual ~InspectorSWInfo() {}
};

/**
 * @brief Info if your Component needs a Vector of N Floats.
 */
class InspectorVFloatSWInfo : public InspectorSWInfo
{
public:
    std::vector<float> m_value;

    InspectorVFloatSWInfo(const std::string &label,
                          const std::vector<float> &initialValues) : InspectorSWInfo(label)
    {
        this->m_value = initialValues;
    }
    virtual ~InspectorVFloatSWInfo() {}

    bool NeedsWrittenLabel() override { return true; }
};

/**
 * @brief Info if your Component needs a Enum field.
 */
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

    bool NeedsWrittenLabel() override { return true; }
};

/**
 * @brief Info if your Component needs a String (either readonly or editable)
 */
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

    bool NeedsWrittenLabel() override { return true; }
};


/**
 * @brief Info if your Component needs a File slot to Browse for one
 */
class InspectorFileSWInfo: public InspectorSWInfo
{
public:
    std::string filepath = "";
    std::string fileExtension = "*";

    InspectorFileSWInfo(const std::string& label,
                        const std::string& fileExtension) : InspectorSWInfo(label)
    {
        this->fileExtension = fileExtension;
    }

    virtual ~InspectorFileSWInfo() {}

    bool NeedsWrittenLabel() override { return true; }
};

/**
 * @brief Info if your Component needs a Button
 */
class InspectorButtonSWInfo: public InspectorSWInfo
{
public:
    std::function<void()> onClickFunction;

    InspectorButtonSWInfo(const std::string& label,
                          std::function<void()> onClickFunction) : InspectorSWInfo(label)
    {
        this->onClickFunction = onClickFunction;
    }

    virtual ~InspectorButtonSWInfo() {}

    bool NeedsWrittenLabel() override { return false; }
};

/**
 * @brief The InspectorWidgetInfo class represents a Container
 * for all the WidgetInfo's a Component can contain.
 */
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
