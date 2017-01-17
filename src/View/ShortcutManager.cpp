#include "ShortcutManager.h"

#include "Debug.h"
#include "Application.h"
#include "EditorWindow.h"
#include "SingletonManager.h"
#include "IShortcutListener.h"

ShortcutManager::ShortcutManager()
{
}

void ShortcutManager::OnKeyPressed(Input::Key key)
{
    if (m_pressedKeys.find(key) == m_pressedKeys.end()) // If it wasn't pressed
    {
        m_pressedKeys.insert(key);
        for (IShortcutListener *list : ShortcutManager::m_shortcutsListeners)
        {
            list->OnShortcutPressedKey(key);
            list->OnShortcutPressed();
        }
    }
    String set = "";
    for (auto it = m_pressedKeys.begin(); it != m_pressedKeys.end(); ++it)
    {
        set += String::ToString(*it) + ", ";
    }
}

void ShortcutManager::OnKeyReleased(Input::Key key)
{
    m_pressedKeys.erase(key);
}

void ShortcutManager::RegisterListener(IShortcutListener *list)
{
    m_shortcutsListeners.PushBack(list);
}

void ShortcutManager::UnregisterListener(IShortcutListener *list)
{
    m_shortcutsListeners.Remove(list);
}

void ShortcutManager::InitFromMainBinary()
{
    SingletonManager::GetInstance()->SetShortcutManagerSingleton(new ShortcutManager());
}

ShortcutManager *ShortcutManager::GetInstance()
{
    return SingletonManager::GetInstance()->GetShortcutManagerSingleton();
}

bool ShortcutManager::IsPressed(Input::Key key)
{
    const Array<Input::Key> keys = {key};
    return ShortcutManager::IsPressed(keys);
}

bool ShortcutManager::IsPressed(const Array<Input::Key> &keys)
{
    ShortcutManager *sm = ShortcutManager::GetInstance();
    ASSERT(sm, "", return false);

    for (Input::Key key : keys)
    {
        if (sm->m_pressedKeys.find(key) == sm->m_pressedKeys.end())
        {
            return false;
        }
    }
    return true;
}

void ShortcutManager::Clear()
{
    m_pressedKeys.clear();
}

