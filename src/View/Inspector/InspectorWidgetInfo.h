#ifndef LISTINSPECTORITEMINFO_H
#define LISTINSPECTORITEMINFO_H

#include <vector>
#include <string>
#include <functional>

#include "Logger.h"

class InspectorSWInfo
{
protected:
    InspectorSWInfo(const std::string &label) : m_label(label)
    {
    }

    virtual void SetValue(void *value) = 0;

public:
    std::string m_label;

    /**
     * @brief Does this Component needs its label printed on top of it?
     * @return
     */
    virtual bool NeedsWrittenLabel() = 0;

    void SetIntValue(const int &value)
    {
        int tmp = value;
        SetValue(&tmp);
    }
    void SetStringValue(const std::string &value)
    {
        std::string tmp = value;
        SetValue(&tmp);
    }
    void SetFloatValue(const float &value)
    {
        std::vector<float> tmp = {value};
        SetValue(&tmp);
    }
    void SetVector2Value(const glm::vec2 &value)
    {
        std::vector<float> tmp = {value.x, value.y};
        SetValue(&tmp);
    }
    void SetVector3Value(const Vector3 &value)
    {
        std::vector<float> tmp = {value.x, value.y, value.z};
        SetValue(&tmp);
    }
    void SetVector4Value(const glm::vec4 &value)
    {
        std::vector<float> tmp = {value.x, value.y, value.z, value.w};
        SetValue(&tmp);
    }
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

    void SetValue(void *value) override
    {
        m_value = *(static_cast<std::vector<float>*>(value));
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

    void SetValue(void *index) override
    {
        m_selectedValueIndex = *(static_cast<int*>(index));
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

    void SetValue(void *value) override
    {
        m_value = *(static_cast< std::string* >(value));
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

    void SetValue(void *filepath) override
    {
        m_filepath = *(static_cast< std::string* >(filepath));
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

    void SetValue(void *onClickFunction) override
    {
        m_onClickFunction = *(static_cast< std::function<void()>* >(onClickFunction));
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

    float GetFloat(const std::string &slotName) const
    {
        return static_cast<InspectorVFloatSWInfo*>(slotInfos[slotName])->m_value[0];
    }
    glm::vec2 GetVector2(const std::string &slotName) const
    {
        InspectorVFloatSWInfo *info = static_cast<InspectorVFloatSWInfo*>(slotInfos[slotName]);
        return {info->m_value[0], info->m_value[1]};
    }
    Vector3 GetVector3(const std::string &slotName) const
    {
        InspectorVFloatSWInfo *info = static_cast<InspectorVFloatSWInfo*>(slotInfos[slotName]);
        return {info->m_value[0], info->m_value[1], info->m_value[2]};
    }
    glm::vec4 GetVector4(const std::string &slotName) const
    {
        InspectorVFloatSWInfo *info = static_cast<InspectorVFloatSWInfo*>(slotInfos[slotName]);
        return {info->m_value[0], info->m_value[1], info->m_value[2], info->m_value[3]};
    }
    std::string GetString(const std::string &slotName) const
    {
        return static_cast<InspectorStringSWInfo*>(slotInfos[slotName])->m_value;
    }
    InspectorVFloatSWInfo* GetVFloatSWSlotInfo(const std::string &slotName) const
    {
        return static_cast<InspectorVFloatSWInfo*>(slotInfos[slotName]);
    }
    InspectorEnumSWInfo* GetEnumSWSlotInfo(const std::string &slotName) const
    {
        return static_cast<InspectorEnumSWInfo*>(slotInfos[slotName]);
    }
    InspectorStringSWInfo* GetStringSWSlotInfo(const std::string &slotName) const
    {
        return static_cast<InspectorStringSWInfo*>(slotInfos[slotName]);
    }
    InspectorFileSWInfo* GetFileSWSlotInfo(const std::string &slotName) const
    {
        return static_cast<InspectorFileSWInfo*>(slotInfos[slotName]);
    }
    InspectorButtonSWInfo* GetButtonSWSlotInfo(const std::string &slotName) const
    {
        return static_cast<InspectorButtonSWInfo*>(slotInfos[slotName]);
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
