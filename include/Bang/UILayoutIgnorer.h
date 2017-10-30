#ifndef UILAYOUTIGNORER_H
#define UILAYOUTIGNORER_H

#include "Bang/Component.h"

NAMESPACE_BANG_BEGIN

class UILayoutIgnorer : public Component
{
    COMPONENT(UILayoutIgnorer)

public:
	UILayoutIgnorer();
	virtual ~UILayoutIgnorer();

    void SetIgnoreLayout(bool ignoreLayout);
    bool IsIgnoreLayout() const;

private:
    bool m_ignoreLayout = true;
};

NAMESPACE_BANG_END

#endif // UILAYOUTIGNORER_H_H

