#ifndef UIDIRLAYOUT_H
#define UIDIRLAYOUT_H

#include "Bang/Map.h"
#include "Bang/Component.h"

NAMESPACE_BANG_BEGIN

class UIDirLayout : public Component
{
    COMPONENT(UIDirLayout)

public:

    // Component
    virtual void OnRecalculateLayout() override;

    void SetSpacing(int spacingPx);

    int GetSpacing() const;

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
};

NAMESPACE_BANG_END

#endif // UIDIRLAYOUT_H
