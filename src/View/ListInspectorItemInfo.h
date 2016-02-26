#ifndef LISTINSPECTORITEMINFO_H
#define LISTINSPECTORITEMINFO_H

#include <vector>
#include <string>

class ListInspectorItemInfo
{
public:
    class SlotInfo
    {
    public:

        enum Type
        {
            String = 0,
            Integer,
            Float,
            Vec2Float,
            Vec3Float,
            Vec4Float
        };

        Type type;
        std::string label;
        std::vector<std::string> componentsLabels;

        SlotInfo() {}
        SlotInfo(const std::string &label,
                 Type type,
                 std::vector<std::string> componentsLabels) : type(type),
                                                              label(label),
                                                              componentsLabels(componentsLabels)
        {}

        bool IsVectorTyped()
        {
            return type == Float || type == Vec2Float || type == Vec3Float || type == Vec4Float;
        }

        int GetVectorComponentsCount()
        {
            return (type - Float) + 1; //enums assign ints consecutive (1,2,3,4,5,6,7...)
        }
    };

    std::vector<SlotInfo> slotInfos;
    ListInspectorItemInfo();
};

#endif // LISTINSPECTORITEMINFO_H
