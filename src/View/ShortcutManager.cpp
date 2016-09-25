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
}

void ShortcutManager::OnKeyReleased(Input::Key key)
{
    auto it = m_pressedKeys.find(key);
    if (it != m_pressedKeys.end())
    {
        m_pressedKeys.erase(it);
    }
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

bool ShortcutManager::IsPressed(const Array<Input::Key> keys)
{
    ShortcutManager *sm = ShortcutManager::GetInstance();
    if (!sm) { return false; }

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
    //m_pressedKeys.clear();
}

