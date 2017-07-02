#ifndef SINGLETONMANAGER_H
#define SINGLETONMANAGER_H

#include "Bang/Time.h"
#include "Bang/Input.h"
#include "Bang/Object.h"
#include "Bang/TypeMap.h"
#include "Bang/IWindow.h"
#include "Bang/Paths.h"

class IO;
class Time;
class Input;
class Console;
class IWindow;
class EditorWindow;
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
    static T* Set(T* singleton)
    {
        if (!SingletonManager::s_mainBinarySM) { return nullptr; }
        SingletonManager::s_mainBinarySM->m_singletons.Set<T>(singleton);
        return singleton;
    }

    template<class T>
    static bool Exists()
    {
        return SingletonManager::s_mainBinarySM->m_singletons.ContainsKey<T>();
    }

    /**
     * The Get methods should only be called by the method that
     * creates dynamically a user Behaviour.
     */
    template<class T>
    static T* Get()
    {
        if (!SingletonManager::s_mainBinarySM) { return nullptr; }
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

    friend class Behaviour;
};

#endif // SINGLETONMANAGER_H
