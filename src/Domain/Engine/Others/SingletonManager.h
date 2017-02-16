#ifndef SINGLETONMANAGER_H
#define SINGLETONMANAGER_H

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

class Time;
class Input;
class Console;
class IWindow;
class Persistence;
class EditorWindow;
class ShortcutManager;
class SingletonManager
{
private:

    static SingletonManager *s_mainBinarySM;

    IWindow *m_mainBinaryWindow = nullptr;
    Time *m_mainBinaryTime = nullptr;
    Input *m_mainBinaryInput = nullptr;
    Persistence *m_mainBinaryPersistence = nullptr;

    #ifdef BANG_EDITOR
    ShortcutManager *m_mainBinaryShortcutManager = nullptr;
    #endif

    SingletonManager() {}

public:


    /**
     * The SetXXXXSingleton methods should only be called by the main binary.
     * So, they should be called in the main() function.
     */

    void SetWindowSingleton(IWindow* mainBinaryEditorWindow);
    void SetTimeSingleton(Time* mainBinaryTime);
    void SetInputSingleton(Input* mainBinaryInput);
    void SetPersistenceSingleton(Persistence* mainBinaryPersistence);

    #ifdef BANG_EDITOR
    void SetShortcutManagerSingleton(ShortcutManager* mainBinaryShortcutManager);
    #endif

    /**
     * The GetXXXXSingleton methods should only be called by the method that
     * creates dynamically a user Behaviour.
     */

    IWindow* GetWindowSingleton();
    Time* GetTimeSingleton();
    Input* GetInputSingleton();
    Persistence* GetPersistenceSingleton();

    #ifdef BANG_EDITOR
    ShortcutManager* GetShortcutManagerSingleton();
    #endif


    /**
     * @brief This method should be called ONLY by the behaviour libraries.
     * This will link the main binary singleton's with the behaviour singleton's.
     */
    static void SetSingletonManagerInstanceFromBehaviourLibrary(SingletonManager *mainBinarySM);


    /**
     * @brief This method should be called ONLY by the main binary.
     */
    static void InitSingletonManagerFromMainBinary();

    /**
     * @brief This method should be called ONLY by the main binary, and passed
     * to the shared libraries as a parameter of the createFunction of the user
     * Behaviours (as a SingletonManager* pointer).
     *
     * If we try to get it from a shared library, it will return another pointer,
     * which won't be the one being used in the main binary, so this class would
     * be useless
     */
    static SingletonManager* GetInstance();
};

#endif // SINGLETONMANAGER_H
