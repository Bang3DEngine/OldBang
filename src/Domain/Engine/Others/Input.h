#ifndef INPUT_H
#define INPUT_H

#include <vector>

#include <QKeyEvent>

#include "Bang.h"

#include "glm/glm.hpp"
#include "Vector2.h"

class Input
{
friend class Screen;
friend class Application;

public:
    enum Key
    {
        Return = 0x01000004, Enter = 0x01000005, Insert = 0x01000006,
        Delete = 0x01000007, Pause = 0x01000008, Print = 0x01000009,
        SysReq = 0x0100000a, Clear = 0x0100000b, Home = 0x01000010,
        End = 0x01000011, Left = 0x01000012, Up = 0x01000013,
        Right = 0x01000014, Down = 0x01000015, PageUp = 0x01000016,
        PageDown = 0x01000017, Shift = 0x01000020, Control = 0x01000021,
        Meta = 0x01000022, Alt = 0x01000023, CapsLock = 0x01000024,
        NumLock = 0x01000025, ScrollLock = 0x01000026,
        F1 = 0x01000030, F2 = 0x01000031, F3 = 0x01000032,
        F4 = 0x01000033, F5 = 0x01000034, F6 = 0x01000035,
        F7 = 0x01000036, F8 = 0x01000037, F9 = 0x01000038,
        F10 = 0x01000039, F11 = 0x0100003a, F12 = 0x0100003b,
        F13 = 0x0100003c, F14 = 0x0100003d, F15 = 0x0100003e,
        F16 = 0x0100003f, F17 = 0x01000040, F18 = 0x01000041,
        F19 = 0x01000042, F20 = 0x01000043, F21 = 0x01000044,
        F22 = 0x01000045, F23 = 0x01000046, F24 = 0x01000047,
        F25 = 0x01000048, F26 = 0x01000049, F27 = 0x0100004a,
        F28 = 0x0100004b, F29 = 0x0100004c, F30 = 0x0100004d,
        F31 = 0x0100004e, F32 = 0x0100004f, F33 = 0x01000050,
        F34 = 0x01000051, F35 = 0x01000052, Super_L = 0x01000053,
        Super_R = 0x01000054, Menu = 0x01000055, Hyper_L = 0x01000056,
        Hyper_R = 0x01000057, Help = 0x01000058, Direction_L = 0x01000059,
        Direction_R = 0x01000060, Space = 0x20, Any = Space,
        Exclam = 0x21, QuoteDbl = 0x22, NumberSign = 0x23,
        Dollar = 0x24, Percent = 0x25, Ampersand = 0x26,
        Apostrophe = 0x27, ParenLeft = 0x28, ParenRight = 0x29,
        Asterisk = 0x2a, Plus = 0x2b, Comma = 0x2c,
        Minus = 0x2d, Period = 0x2e, Slash = 0x2f,
        Num0 = 0x30, Num1 = 0x31, Num2 = 0x32, Num3 = 0x33,
        Num4 = 0x34, Num5 = 0x35, Num6 = 0x36, Num7 = 0x37,
        Num8 = 0x38, Num9 = 0x39, Colon = 0x3a, Semicolon = 0x3b,
        Less = 0x3c, Equal = 0x3d, Greater = 0x3e, Question = 0x3f,
        At = 0x40, A = 0x41, B = 0x42, C = 0x43, D = 0x44, E = 0x45, F = 0x46,
        G = 0x47, H = 0x48, I = 0x49, J = 0x4a, K = 0x4b, L = 0x4c, M = 0x4d,
        N = 0x4e, O = 0x4f, P = 0x50, Q = 0x51, R = 0x52, S = 0x53, T = 0x54,
        U = 0x55, V = 0x56, W = 0x57, X = 0x58, Y = 0x59, Z = 0x5a,
        BracketLeft = 0x5b, Backslash = 0x5c, BracketRight = 0x5d,
        AsciiCircum = 0x5e, Underscore = 0x5f, QuoteLeft = 0x60,
        BraceLeft = 0x7b, Bar = 0x7c, BraceRight = 0x7d, AsciiTilde = 0x7e,
        Nobreakspace = 0x0a0, exclamdown = 0x0a1, cent = 0x0a2,
        Sterling = 0x0a3, currency = 0x0a4, yen = 0x0a5,
        Brokenbar = 0x0a6, section = 0x0a7, diaeresis = 0x0a8,
        Copyright = 0x0a9, ordfeminine = 0x0aa, guillemotleft = 0x0ab,
        Notsign = 0x0ac, hyphen = 0x0ad, registered = 0x0ae, macron = 0x0af,
        Degree = 0x0b0, plusminus = 0x0b1, twosuperior = 0x0b2,
        Threesuperior = 0x0b3, acute = 0x0b4, mu = 0x0b5, paragraph = 0x0b6,
        Periodcentered = 0x0b7, cedilla = 0x0b8, onesuperior = 0x0b9,
        Masculine = 0x0ba, Guillemotright = 0x0bb, onequarter = 0x0bc,
        Onehalf = 0x0bd, Threequarters = 0x0be, Questiondown = 0x0bf,
        Agrave = 0x0c0, Aacute = 0x0c1, Acircumflex = 0x0c2,
        Atilde = 0x0c3, Adiaeresis = 0x0c4, Aring = 0x0c5,
        AE = 0x0c6, Ccedilla = 0x0c7, Egrave = 0x0c8,
        Eacute = 0x0c9, Ecircumflex = 0x0ca, Ediaeresis = 0x0cb,
        Igrave = 0x0cc, Iacute = 0x0cd, Icircumflex = 0x0ce,
        Idiaeresis = 0x0cf, ETH = 0x0d0, Ntilde = 0x0d1, Ograve = 0x0d2,
        Oacute = 0x0d3, Ocircumflex = 0x0d4, Otilde = 0x0d5,
        Odiaeresis = 0x0d6, Multiply = 0x0d7, Ooblique = 0x0d8,
        Ugrave = 0x0d9, Uacute = 0x0da, Ucircumflex = 0x0db,
        Udiaeresis = 0x0dc, Yacute = 0x0dd, Thorn = 0x0de,
        Ssharp = 0x0df, Division = 0x0f7, Ydiaeresis = 0x0ff,
        AltGr= 0x01001103, Multi_key= 0x01001120,
        Codeinput= 0x01001137, SingleCandidate= 0x0100113c,
        MultipleCandidate= 0x0100113d, PreviousCandidate= 0x0100113e,
        Mode_switch= 0x0100117e, Unknown = 0x01ffffff
    };

    enum MouseButton
    {
        MNoButton         = 0x00000000,
        MLeft             = 0x00000001,
        MRight            = 0x00000002,
        MMiddle           = 0x00000004,
        MXButton1         = 0x00000008,
        MXButton2         = 0x00000010,
        MMouseButtonMask  = 0x000000ff
    };

private:

    Input();

    /**
     * @brief Struct to keep track of a Button state.
     * A button can either be a key or a mouse button,
     * and it contains the states up, down and pressed.
     */
    struct ButtonInfo
    {
        bool up;   //just one frame
        bool down; //just one frame
        bool pressed; //long duration

        ButtonInfo() { up = down = pressed = false; }
        ButtonInfo(bool up, bool down, bool pressed)
        {
            this->up = up;
            this->down = down;
            this->pressed = pressed;
        }
    };

    /**
     * @brief For every Key, we have a ButtonInfo
     * assigned to it
     */
    std::map<Key, ButtonInfo> m_keyInfos;


    /**
     * @brief If true, mouse will loop in x and y when
     * going outside the Screen.
     */
    bool m_mouseWrapping = false;

    /**
     * @brief isADoubleClick will be true in the frame where
     * there's the second click of a double click
     */
    bool m_isADoubleClick = false;

    /**
     * @brief Used to handle double click timing
     */
    float m_secsSinceLastMouseDown = 0.0f;

    /**
     * @brief Max time in seconds between 2 clicks to
     * be considered double-click
     */
    const float c_doubleClickMaxSeconds = 0.3f;
    float m_lastMouseWheelDelta = 0.0f;
    bool m_lockMouseMovement = false;

    /**
     * @brief Used to fix an issue with lockMouseMovement
     */
    int m_framesMouseStopped = 0  ;

    /**
     * @brief Mouse coordinates in Screen space.
     */
    Vector2 m_mouseCoords, m_lastMouseCoords;


    /**
     * @brief For every MouseButton, we have a ButtonInfo
     * assigned to it
     */
    std::map<MouseButton, ButtonInfo> m_mouseInfo;

public:
    static void InitFromMainBinary();

private:
    void OnNewFrame();

    void HandleMouseWrapping();
    void HandleInputMouseWheel(QWheelEvent *event);
    void HandleInputMouseMove(QMouseEvent *event);
    void HandleInputMousePress(QMouseEvent *event);
    void HandleInputMouseRelease(QMouseEvent *event);
    void HandleInputKeyPress(QKeyEvent *event);
    void HandleInputKeyReleased(QKeyEvent *event);

public:
    static Input* GetInstance();

    static bool GetMouseWrapping();
    static void SetMouseWrapping(bool m_mouseWrapping);

    static bool GetKey(Key k);
    static bool GetKeyUp(Key k);
    static bool GetKeyDown(Key k);

    static float GetMouseWheel();

    static bool GetMouseButton(MouseButton mb);
    static bool GetMouseButtonUp(MouseButton mb);
    static bool GetMouseButtonDown(MouseButton mb);
    static bool GetMouseButtonDoubleClick(MouseButton mb);

    /**
     * @brief GetMouseAxisX
     * @return Mouse movement in x divided by screen width [0.0f, 1.0f]
     */
    static float GetMouseAxisX();

    /**
     * @brief GetMouseDeltaY
     * @return Mouse movement in y divided by screen height [0.0f, 1.0f]
     */
    static float GetMouseAxisY();
    static Vector2 GetMouseAxis();

    /**
     * @brief GetMouseDeltaX
     * @return Mouse movement in x in pixels [0.0f, screenWidth]
     */
    static float GetMouseDeltaX();

    /**
     * @brief GetMouseDeltaY
     * @return Mouse movement in y in pixels [0.0f, screenHeight]
     */
    static float GetMouseDeltaY();
    static Vector2 GetMouseDelta();

    static void LockMouseMovement(bool lock);
    static bool IsLockMouseMovement();

    static Vector2 GetMouseCoords();
};

#endif // INPUT_H
