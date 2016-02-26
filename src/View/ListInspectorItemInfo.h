#ifndef LISTINSPECTORITEMINFO_H
#define LISTINSPECTORITEMINFO_H

#include <vector>
#include <string>

class ListInspectorItemInfoSlot
{
public:
    enum Type
    {
        String = 0,
        Integer,
        Float, Vec2Float, Vec3Float, Vec4Float
    };

    Type type;
    std::string label;
    std::vector<std::string> componentsLabels;

protected:
    ListInspectorItemInfoSlot(const std::string &label,
                              Type type,
                              std::vector<std::string> componentsLabels) : type(type),
                                                                           label(label),
                                                                           componentsLabels(componentsLabels) { }
public:
    bool IsVectorTyped()
    { return type == Float || type == Vec2Float || type == Vec3Float || type == Vec4Float; }

    int GetVectorComponentsCount()
    { return (type - Float) + 1; /*enums assign ints consecutive (1,2,3,4,5,6,7...)*/ }
};




class ListInspectorItemInfoSlotVecFloat : public ListInspectorItemInfoSlot
{
public:
    std::vector<float> initialValues;
    ListInspectorItemInfoSlotVecFloat(const std::string &label,
                                      ListInspectorItemInfoSlot::Type type,
                                      const std::vector<float> &initialValues,
                                      std::vector<std::string> componentsLabels) :
                                                ListInspectorItemInfoSlot(label, type, componentsLabels)
    {
        this->initialValues = initialValues;
    }
};




class ListInspectorItemInfo
{
public:
    std::vector<ListInspectorItemInfoSlot*> slotInfos;
    ListInspectorItemInfo();
};

#endif // LISTINSPECTORITEMINFO_H
