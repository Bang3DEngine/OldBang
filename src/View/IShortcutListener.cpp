#include "IShortcutListener.h"

#include "ShortcutManager.h"

IShortcutListener::IShortcutListener()
{
    ShortcutManager::RegisterListener(this);
}

IShortcutListener::~IShortcutListener()
{
    ShortcutManager::UnregisterListener(this);
}
