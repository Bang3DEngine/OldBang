#ifndef UICOMBOBOX_H
#define UICOMBOBOX_H

#include "Bang/Component.h"
#include "Bang/IEventEmitter.h"
#include "Bang/IFocusListener.h"
#include "Bang/IValueChangedListener.h"

NAMESPACE_BANG_BEGIN

class UIComboBox : public Component,
                   public EventEmitter<IValueChangedListener>,
                   public EventEmitter<IFocusListener>,
                   public IFocusListener
{
    COMPONENT(UIComboBox)

public:
    void AddItem(const String &label, int value);

private:
	UIComboBox();
    virtual ~UIComboBox();

    void OnUpdate() override;

    static UIComboBox *CreateInto(GameObject *go);

    friend class GameObjectFactory;
};

NAMESPACE_BANG_END

#endif // UICOMBOBOX_H

