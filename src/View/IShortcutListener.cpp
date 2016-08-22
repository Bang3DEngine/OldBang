#include "IShortcutListener.h"

#include "Application.h"
#include "ShortcutManager.h"

IShortcutListener::IShortcutListener()
{
    ShortcutManager::GetInstance()->RegisterListener(this);
}

IShortcutListener::~IShortcutListener()
{
    ShortcutManager::GetInstance()->UnregisterListener(this);
}

void IShortcutListener::OnShortcutPressedKey(Input::Key key)
{
}
