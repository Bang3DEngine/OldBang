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
    if (!m_pressedKeys.Contains(key)) // If it wasn't pressed
    {
        m_pressedKeys.Insert(key);
        for (IShortcutListener *list : ShortcutManager::m_shortcutsListeners)
        {
            if (!QApplication::activeModalWidget()) // Reject if modal/s over
            {
                list->OnShortcutPressedKey(key);
                list->OnShortcutPressed();
            }
        }
    }
    String set = "";
    for (auto it = m_pressedKeys.Begin(); it != m_pressedKeys.End(); ++it)
    {
        set += String::ToString( int(*it) ) + ", ";
    }
}

void ShortcutManager::OnKeyReleased(Input::Key key)
{
    m_pressedKeys.Remove(key);
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
    SingletonManager::Set<ShortcutManager>(new ShortcutManager());
}

ShortcutManager *ShortcutManager::GetInstance()
{
    return SingletonManager::Get<ShortcutManager>();
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
        if (!sm->m_pressedKeys.Contains(key))
        {
            return false;
        }
    }
    return true;
}

void ShortcutManager::Clear()
{
    m_pressedKeys.Clear();
}

