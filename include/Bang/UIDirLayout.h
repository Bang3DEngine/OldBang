#ifndef UIDIRLAYOUT_H
#define UIDIRLAYOUT_H

#include "Bang/Map.h"
#include "Bang/Component.h"

class UIDirLayout : public Component
{
    COMPONENT(UIDirLayout)

public:

    // Component
    virtual void OnUpdate() override;

    void SetSpacing(int spacingPx);
    void SetStretch(int index, float stretch);
    void SetStretch(GameObject *gameObject, float stretch);

    float GetStretch(int index) const;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

protected:
    UIDirLayout();
    UIDirLayout(bool vertical);
    virtual ~UIDirLayout();

private:
    int m_spacingPx = 0;
    bool m_vertical = false;
    Map<int, float> m_stretches;
};

#endif // UIDIRLAYOUT_H
