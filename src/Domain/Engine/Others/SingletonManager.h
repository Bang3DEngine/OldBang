#ifndef SINGLETONMANAGER_H
#define SINGLETONMANAGER_H

/**
 * @brief This class manages the global Singleton's across
 * the program and the shared libraries of the custom Behaviours.
 *
 * When adding a custom user Behaviour, it must use the global
 * Singleton's that were in use by the main program (the Logger,
 * the WindowMain, etc.)
 *
 * So, every time a user Behaviour is linked to the main binary,
 * the SingletonManager must initialize its references to the ones being
 * used in the main binary (pointerInShared = pointerInMain)
 *
 * So, the only thing we will pass to the shared library when creating
 * a dynamic Behaviour, is a pointer to the SingletonManager created
 * in the MAIN BINARY.
 */

class WindowMain;
class Logger;
class Time;
class SingletonManager
{
private:

    static SingletonManager *mainBinarySM;

    WindowMain *mainBinaryWindowMain = nullptr;
    Logger *mainBinaryLogger = nullptr;
    Time *mainBinaryTime = nullptr;

    SingletonManager() {}

public:


    /**
     * The SetXXXXSingleton methods should only be called by the main binary.
     * So, they should be called in the main() function.
     */

    void SetWindowMainSingleton(WindowMain* mainBinaryWM);
    void SetLoggerSingleton(Logger* mainBinaryL);
    void SetTimeSingleton(Time* mainBinaryT);

    /**
     * The GetXXXXSingleton methods should only be called by the method that
     * creates dynamically a user Behaviour.
     */

    WindowMain* GetWindowMainSingleton();
    Logger* GetLoggerSingleton();
    Time* GetTimeSingleton();

    /**
     * @brief This method should be called ONLY by the behaviour libraries.
     * This will link the main binary singleton's with the behaviour singleton's.
     */
    static SingletonManager* SetInstanceFromBehaviourLibrary(
                                    SingletonManager *mainBinarySM);


    /**
     * @brief This method should be called ONLY by the main binary.
     */
    static SingletonManager* Init();

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