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
};

/**
 * @brief Info if your Component needs a Vector of N Floats.
 */
class InspectorVFloatSWInfo : public InspectorSWInfo
{
public:
    std::vector<float> m_value;

    InspectorVFloatSWInfo(const std::string &label, int numComponents) : InspectorSWInfo(label)
    {
        m_value.resize(numComponents);
    }

    bool NeedsWrittenLabel() override { return true; }
};

/**
 * @brief Info if your Component needs a Enum field.
 */
class InspectorEnumSWInfo : public InspectorSWInfo
{
public:
    std::vector<std::string> m_enumValues;
    int m_selectedValueIndex = 0;

    InspectorEnumSWInfo(const std::string &label,
                        const std::vector<std::string> &enumValues)
                        : InspectorSWInfo(label)
    {
        m_enumValues = enumValues;
    }

    bool NeedsWrittenLabel() override { return true; }
};

/**
 * @brief Info if your Component needs a String (either readonly or editable)
 */
class InspectorStringSWInfo: public InspectorSWInfo
{
public:

    bool m_readonly = false, m_inlined = false;
    std::string m_value = "";

    InspectorStringSWInfo(const std::string& label,
                          bool readonly = false, bool inlined = false) : InspectorSWInfo(label)
    {
        m_value = "";
        m_readonly = readonly;
        m_inlined = inlined;
    }

    bool NeedsWrittenLabel() override { return true; }
};


/**
 * @brief Info if your Component needs a File slot to Browse for one
 */
class InspectorFileSWInfo: public InspectorSWInfo
{
public:
    std::string m_filepath = "";
    std::string m_fileExtension = "*";

    InspectorFileSWInfo(const std::string& label,
                        const std::string& fileExtension) : InspectorSWInfo(label)
    {
        m_fileExtension = fileExtension;
    }

    bool NeedsWrittenLabel() override { return true; }
};

/**
 * @brief Info if your Component needs a Button
 */
class InspectorButtonSWInfo: public InspectorSWInfo
{
public:
    std::function<void()> m_onClickFunction;

    InspectorButtonSWInfo(const std::string& label,
                          std::function<void()> onClickFunction) : InspectorSWInfo(label)
    {
        m_onClickFunction = onClickFunction;
    }

    bool NeedsWrittenLabel() override { return false; }
};

/**
 * @brief The InspectorWidgetInfo class represents a Container
 * for all the WidgetInfo's a Component can contain.
 */
class InspectorWidgetInfo
{
private:
    mutable std::map<std::string, InspectorSWInfo*> slotInfos;

public:

    InspectorWidgetInfo() {}
    virtual ~InspectorWidgetInfo() {}

    InspectorSWInfo* GetSlotInfo(const std::string &slotName) const
    {
        return slotInfos[slotName];
    }

    const std::map<std::string, InspectorSWInfo*>& GetSlotInfos() const
    {
        return slotInfos;
    }

    void AddSlotInfo(InspectorSWInfo* slotInfo)
    {
        slotInfos[slotInfo->m_label] = slotInfo;
    }

    void AddSlotInfos(const std::vector<InspectorSWInfo*> &slotInfos)
    {
        for (InspectorSWInfo *slotInfo : slotInfos)
        {
            AddSlotInfo(slotInfo);
        }
    }
};

#endif // LISTINSPECTORITEMINFO_H
