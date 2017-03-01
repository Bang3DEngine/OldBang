#ifndef SINGLETONMANAGER_H
#define SINGLETONMANAGER_H

#include "Time.h"
#include "Input.h"
#include "Object.h"
#include "TypeMap.h"
#include "IWindow.h"
#include "Persistence.h"

class Time;
class Input;
class Console;
class IWindow;
class Persistence;
class EditorWindow;
class ShortcutManager;
/**
 * @brief This class manages the global Singleton's across
 * the program and the shared libraries of the custom Behaviours.
 *
 * When adding a custom user Behaviour, it must use the global
 * Singleton's that were in use by the main program (the Console,
 * the EditorWindow, etc.)
 *
 * So, every time a user Behaviour is linked to the main binary,
 * the SingletonManager must initialize its references to the ones being
 * used in the main binary (pointerInShared = pointerInMain)
 *
 * So, the only thing we will pass to the shared library when creating
 * a dynamic Behaviour, is a pointer to the SingletonManager created
 * in the MAIN BINARY.
 */

class SingletonManager
{
public:
    /**
     * The Set methods should only be called by the main binary.
     * So, they should be called in the main() function.
     */
    template<class T>
    static void Set(T* singleton)
    {
        return SingletonManager::s_mainBinarySM->m_singletons.Set<T>(singleton);
    }

    /**
     * The Get methods should only be called by the method that
     * creates dynamically a user Behaviour.
     */
    template<class T>
    static T* Get()
    {
        return Object::SCast<T>(
                    SingletonManager::s_mainBinarySM->m_singletons.Get<T>());
    }

    /**
     * @brief This method should be called ONLY by the behaviour libraries.
     * This will link the main binary singleton's with the behaviour singleton's.
     */
    static void SetSingletonManagerInstanceFromBehaviourLibrary(
            SingletonManager *mainBinarySM);


    /**
     * @brief This method should be called ONLY by the main binary.
     */
    static void InitSingletonManagerFromMainBinary();


private:
    static SingletonManager *s_mainBinarySM;
    TypeMap<void*> m_singletons;

    SingletonManager();

    friend class BehaviourHolder;
};

#endif // SINGLETONMANAGER_H
