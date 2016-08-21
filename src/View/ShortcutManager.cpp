#include "ShortcutManager.h"

std::list<IShortcutListener*> ShortcutManager::m_shortcutsListeners;
std::set<Input::Key> ShortcutManager::m_pressedKeys;

ShortcutManager::ShortcutManager()
{
}

void ShortcutManager::OnKeyPressed(Input::Key key)
{
    ShortcutManager::m_pressedKeys.insert(key);
    ShortcutManager::NotifyListeners();
}

void ShortcutManager::OnKeyReleased(Input::Key key)
{
    auto it = ShortcutManager::m_pressedKeys.find(key);
    if (it != ShortcutManager::m_pressedKeys.end())
    {
        ShortcutManager::m_pressedKeys.erase(it);
    }
    ShortcutManager::NotifyListeners();
}

void ShortcutManager::RegisterListener(IShortcutListener *list)
{
    ShortcutManager::m_shortcutsListeners.push_back(list);
}

void ShortcutManager::UnregisterListener(IShortcutListener *list)
{
    ShortcutManager::m_shortcutsListeners.remove(list);
}

void ShortcutManager::NotifyListeners()
{
    for (IShortcutListener *list : ShortcutManager::m_shortcutsListeners)
    {
        list->OnShortcutsUpdate();
    }
}

bool ShortcutManager::IsPressed(Input::Key key)
{
    const std::vector<Input::Key> keys = {key};
    return ShortcutManager::IsPressed(keys);
}

bool ShortcutManager::IsPressed(const std::vector<Input::Key> keys)
{
    for (Input::Key key : keys)
    {
        if (ShortcutManager::m_pressedKeys.find(key) ==
               ShortcutManager::m_pressedKeys.end())
        {
            return false;
        }
    }
    return true;
}
