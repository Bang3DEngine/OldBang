#ifndef UIGROUPLAYOUT_H
#define UIGROUPLAYOUT_H

#include "Bang/Stretch.h"
#include "Bang/Alignment.h"
#include "Bang/Component.h"
#include "Bang/ILayoutElement.h"
#include "Bang/ILayoutController.h"

NAMESPACE_BANG_BEGIN

class UIGroupLayout : public Component,
                      public ILayoutElement,
                      public ILayoutController
{
public:
    UIGroupLayout();
    virtual ~UIGroupLayout();

    void SetSpacing(int spacingPx);
    void SetPaddingLeft(int paddingLeft);
    void SetPaddingBot(int paddingBot);
    void SetPaddingRight(int paddingRight);
    void SetPaddingTop(int paddingTop);
    void SetPaddings(int padding);
    void SetPaddings(int paddingLeft, int paddingBot,
                     int paddingRight, int paddingTop);
    void SetChildrenHorizontalAlignment(HorizontalAlignment hAlignment);
    void SetChildrenVerticalAlignment(VerticalAlignment vAlignment);
    void SetChildrenHorizontalStretch(Stretch hStretch);
    void SetChildrenVerticalStretch(Stretch vStretch);

    int GetSpacing() const;
    int GetPaddingLeft()  const;
    int GetPaddingBot()   const;
    int GetPaddingRight() const;
    int GetPaddingTop()   const;
    const Vector2i &GetPaddingLeftBot()  const;
    const Vector2i &GetPaddingRightTop() const;
    Vector2i GetPaddingSize() const;
    HorizontalAlignment GetChildrenHorizontalAlignment() const;
    VerticalAlignment GetChildrenVerticalAlignment() const;
    Stretch GetChildrenVerticalStretch() const;
    Stretch GetChildrenHorizontalStretch() const;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

private:
    int m_spacingPx = 0;
    HorizontalAlignment m_childrenHorizontalAlignment = HorizontalAlignment::Center;
    VerticalAlignment   m_childrenVerticalAlignment   = VerticalAlignment::Center;
    Stretch m_childrenHorizontalStretch = Stretch::Full;
    Stretch m_childrenVerticalStretch   = Stretch::Full;

    Vector2i m_paddingLeftBot  = Vector2i::Zero;
    Vector2i m_paddingRightTop = Vector2i::Zero;
};

NAMESPACE_BANG_END

#endif // UIGROUPLAYOUT_H
