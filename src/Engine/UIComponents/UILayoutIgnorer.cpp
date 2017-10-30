#include "Bang/UILayoutIgnorer.h"

USING_NAMESPACE_BANG

UILayoutIgnorer::UILayoutIgnorer()
{
}

UILayoutIgnorer::~UILayoutIgnorer()
{
}

void UILayoutIgnorer::SetIgnoreLayout(bool ignoreLayout)
{
    m_ignoreLayout = ignoreLayout;
}

bool UILayoutIgnorer::IsIgnoreLayout() const
{
    return m_ignoreLayout;
}

