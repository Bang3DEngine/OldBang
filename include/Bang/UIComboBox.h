#ifndef UICOMBOBOX_H
#define UICOMBOBOX_H

#include "Bang/Map.h"
#include "Bang/UIList.h"
#include "Bang/Component.h"
#include "Bang/IEventEmitter.h"
#include "Bang/IFocusListener.h"
#include "Bang/IValueChangedListener.h"

NAMESPACE_BANG_BEGIN

FORWARD class UITextRenderer;

class UIComboBox : public Component,
                   public EventEmitter<IValueChangedListener>,
                   public EventEmitter<IFocusListener>,
                   public IFocusListener
{
    COMPONENT(UIComboBox)

public:
    void AddItem(const String &label, int value);
    void SetSelectionByIndex(int index);
    void SetSelectionByValue(int value);

    int GetSelectedValue() const;
    int GetSelectedIndex() const;
    String GetSelectedLabel() const;

private:
    int m_selectedIndex = -1;
    Array<int> m_indexToValue;
    Array<String> m_indexToLabel;

    UIList *p_list = nullptr;
    UITextRenderer *p_currentItemText = nullptr;

	UIComboBox();
    virtual ~UIComboBox();

    void OnUpdate() override;

    UIList *GetList() const;
    static UIComboBox *CreateInto(GameObject *go);

    // UIList item handler
    void OnListSelectionCallback(GameObject *item, UIList::Action action);

    friend class GameObjectFactory;
};

NAMESPACE_BANG_END

#endif // UICOMBOBOX_H

