#include "Bang/UISlider.h"

#include "Bang/Input.h"
#include "Bang/GameObject.h"
#include "Bang/RectTransform.h"
#include "Bang/UIInputNumber.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"

USING_NAMESPACE_BANG


UISlider::UISlider()
{
}

UISlider::~UISlider()
{
}

void UISlider::OnUpdate()
{
    Component::OnUpdate();

    if (GetHandleFocusable()->IsBeingPressed())
    {
        GetHandleRenderer()->SetTint( m_pressedColor );

        RectTransform *handleRT = GetHandleRectTransform();
        float mouseLocalX = handleRT->FromViewportPointToLocalPointNDC(
                                        Input::GetMousePosition() ).x;

        mouseLocalX = Math::Clamp(mouseLocalX, -1.0f, 1.0f);
        float mousePercent = mouseLocalX * 0.5f + 0.5f;
        SetValuePercent(mousePercent);
    }
    else
    {
        GetHandleRenderer()->SetTint( GetHandleFocusable()->IsMouseOver() ?
                                       m_overColor : m_idleColor);
    }
}

void UISlider::OnValueChanged(Object *object)
{
    ASSERT(object == GetInputNumber());
    SetValue( GetInputNumber()->GetValue() );
}

void UISlider::UpdateSliderHandlerFromValue()
{
    GetHandleRectTransform()->SetAnchors( Vector2(GetValuePercent() * 2.0f - 1.0f,
                                                  0) );
}

RectTransform *UISlider::GetHandleRectTransform() const
{
    return p_handleRenderer->GetGameObject()->GetRectTransform();
}

void UISlider::SetValue(float value)
{
    if (value != GetValue())
    {
        float clampedValue = Math::Clamp(value, GetMinValue(), GetMaxValue());
        m_value = clampedValue;

        GetInputNumber()->SetValue( GetValue() );

        EventEmitter<IValueChangedListener>::PropagateToListeners(
                    &IValueChangedListener::OnValueChanged, this);
        UpdateSliderHandlerFromValue();
    }
}

void UISlider::SetMinValue(float minValue)
{
    if (minValue != GetMinValue())
    {
        m_minValue = minValue;
        if (GetMaxValue() < GetMinValue()) { SetMaxValue(GetMinValue()); }
        if (GetValue() < GetMinValue()) { SetValue(GetMinValue()); }
        UpdateSliderHandlerFromValue();
    }
}

void UISlider::SetMaxValue(float maxValue)
{
    if (maxValue != GetMaxValue())
    {
        m_maxValue = maxValue;
        if (GetMinValue() > GetMaxValue()) { SetMinValue(GetMaxValue()); }
        if (GetValue() > GetMaxValue()) { SetValue(GetMaxValue()); }
        UpdateSliderHandlerFromValue();
    }
}

void UISlider::SetMinMaxValues(float minValue, float maxValue)
{
    SetMinValue(minValue);
    SetMaxValue(maxValue);
}

void UISlider::SetValuePercent(float percent)
{
    SetValue( GetMinValue() + (GetMaxValue()-GetMinValue()) * percent);
}

float UISlider::GetValue() const
{
    return m_value;
}

float UISlider::GetMinValue() const
{
    return m_minValue;
}

float UISlider::GetMaxValue() const
{
    return m_maxValue;
}

float UISlider::GetValuePercent() const
{
    float range = GetMaxValue() - GetMinValue();
    float percent = (range != 0.0f) ? ((GetValue() - GetMinValue()) / range) : 0.0f;
    ASSERT(percent >= 0.0f && percent <= 1.0f);
    return percent;
}

UIInputNumber *UISlider::GetInputNumber() const
{
    return p_inputNumber;
}

UIImageRenderer *UISlider::GetGuideRenderer() const
{
    return p_guideRenderer;
}

UIImageRenderer *UISlider::GetHandleRenderer() const
{
    return p_handleRenderer;
}

IFocusable *UISlider::GetHandleFocusable() const
{
    return p_handleFocusable;
}

const Color &UISlider::GetIdleColor() const
{
    return m_idleColor;
}

const Color &UISlider::GetOverColor() const
{
    return m_overColor;
}

const Color &UISlider::GetPressedColor() const
{
    return m_pressedColor;
}

UISlider *UISlider::CreateInto(GameObject *go)
{
    REQUIRE_COMPONENT(go, RectTransform);

    UISlider *slider = go->AddComponent<UISlider>();

    UIHorizontalLayout *hl = go->AddComponent<UIHorizontalLayout>();
    hl->SetSpacing(10);

    UILayoutElement *hlLE = go->AddComponent<UILayoutElement>();
    hlLE->SetFlexibleWidth(1.0f);

    GameObject *sliderContainer = GameObjectFactory::CreateUIGameObject();

    UILayoutElement *sliderLE = sliderContainer->AddComponent<UILayoutElement>();
    sliderLE->SetFlexibleSize( Vector2::One );

    UIImageRenderer *guideRenderer = GameObjectFactory::CreateUIImage(Color::Black);
    RectTransform *guideRT = guideRenderer->GetGameObject()->GetRectTransform();
    guideRT->SetAnchorMin( Vector2(-1.0f, 0.0f) );
    guideRT->SetAnchorMax( Vector2( 1.0f, 0.0f) );
    guideRT->SetMarginTop(-1);

    UIImageRenderer *handleRenderer = GameObjectFactory::CreateUIImage(slider->m_idleColor);
    RectTransform *handleRT = handleRenderer->GetGameObject()->GetRectTransform();
    handleRT->SetAnchors(Vector2::Zero);
    handleRT->SetMargins(-6);

    UIFocusable *handleFocusable = handleRenderer->GetGameObject()
                                   ->AddComponent<UIFocusable>();
    handleFocusable->EventEmitter<IFocusListener>::RegisterListener(slider);

    guideRenderer->GetGameObject()->SetParent(sliderContainer);
    handleRenderer->GetGameObject()->SetParent(sliderContainer);

    UIInputNumber *inputNumber = GameObjectFactory::CreateUIInputNumber();
    UILayoutElement *inputNumberLE = inputNumber->GetGameObject()
                                     ->AddComponent<UILayoutElement>();
    inputNumberLE->SetMinWidth(20);
    inputNumberLE->SetFlexibleWidth( 0.2f );
    inputNumber->EventEmitter<IValueChangedListener>::RegisterListener(slider);

    slider->p_guideRenderer = guideRenderer;
    slider->p_handleRenderer = handleRenderer;
    slider->p_handleFocusable = handleFocusable;
    slider->p_inputNumber = inputNumber;

    slider->UpdateSliderHandlerFromValue();

    sliderContainer->SetParent(go);
    inputNumber->GetGameObject()->SetParent(go);

    return slider;
}

