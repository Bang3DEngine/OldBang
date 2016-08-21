#ifndef SHORTCUTMANAGER_H
#define SHORTCUTMANAGER_H

#include <set>
#include <vector>

#include "Input.h"
#include "IShortcutListener.h"

class ShortcutManager
{
friend class Application;
friend class IShortcutListener;

private:
    static std::list<IShortcutListener*> m_shortcutsListeners;
    static std::set<Input::Key> m_pressedKeys;

    ShortcutManager();

    static void OnKeyPressed(Input::Key key);
    static void OnKeyReleased(Input::Key key);


    static void RegisterListener(IShortcutListener *list);
    static void UnregisterListener(IShortcutListener *list);
    static void NotifyListeners();

public:

    static bool IsPressed(Input::Key key);
    static bool IsPressed(const std::vector<Input::Key> keys);
};

#endif // SHORTCUTMANAGER_H
