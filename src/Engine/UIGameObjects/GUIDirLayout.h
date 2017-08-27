#ifndef UIDIRLAYOUT_H
#define UIDIRLAYOUT_H

#include "Bang/Map.h"
#include "Bang/UIComponent.h"

class GUIDirLayout : public UIComponent
{
    COMPONENT(GUIDirLayout)

public:

    // Component
    virtual void OnUpdate() override;

    void SetSpacing(int spacingPx);
    void SetStretch(int index, float stretch);
    void SetStretch(GameObject *gameObject, float stretch);

    float GetStretch(int index) const;

    // SerializableObject
    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

protected:
    GUIDirLayout();
    GUIDirLayout(bool vertical);
    virtual ~GUIDirLayout();

private:
    int m_spacingPx = 0;
    bool m_vertical = false;
    Map<int, float> m_stretches;
};

#endif // UIDIRLAYOUT_H
