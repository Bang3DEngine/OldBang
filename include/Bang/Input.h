#ifndef INPUT_H
#define INPUT_H

#include <iostream>
#include <SDL2/SDL.h>

#include "Bang/Map.h"
#include "Bang/Array.h"
#include "Bang/String.h"
#include "Bang/Vector2.h"
#include "Bang/IToString.h"

NAMESPACE_BANG_BEGIN

FORWARD class Window;

enum class Key
{
    None   = 0,
    Return = SDLK_RETURN, Enter = SDLK_RETURN, Insert = SDLK_INSERT,
    Delete = SDLK_DELETE, Pause = SDLK_PAUSE, Print = SDLK_PRINTSCREEN,
    Clear = SDLK_CLEAR, Home = SDLK_HOME,
    End = SDLK_END, Left = SDLK_LEFT, Up = SDLK_UP,
    Right = SDLK_RIGHT, Down = SDLK_DOWN, PageUp = SDLK_PAGEUP,
    PageDown = SDLK_PAGEDOWN, LShift = SDLK_LSHIFT, RShift = SDLK_RSHIFT,
    LControl = SDLK_LCTRL, RControl = SDLK_RCTRL,
    LAlt = SDLK_LALT, RAlt = SDLK_RALT,
    Tab = SDLK_TAB, TabKp = SDLK_KP_TAB,
    CapsLock = SDLK_CAPSLOCK, NumLock = SDLK_NUMLOCKCLEAR,
    ScrollLock = SDLK_SCROLLLOCK,
    F1 = SDLK_F1, F2 = SDLK_F2, F3 = SDLK_F3,
    F4 = SDLK_F4, F5 = SDLK_F5, F6 = SDLK_F6,
    F7 = SDLK_F7, F8 = SDLK_F8, F9 = SDLK_F9,
    F10 = SDLK_F10, F11 = SDLK_F11, F12 = SDLK_F12,
    F13 = SDLK_F13, F14 = SDLK_F14, F15 = SDLK_F15,
    F16 = SDLK_F16, F17 = SDLK_F17, F18 = SDLK_F18,
    F19 = SDLK_F19, F20 = SDLK_F20, F21 = SDLK_F21,
    F22 = SDLK_F22, F23 = SDLK_F23, F24 = SDLK_F24,
    LSuper = SDLK_LGUI, RSuper = SDLK_RGUI, Menu = SDLK_MENU,
    Help = SDLK_HELP, Space = SDLK_SPACE,
    Exclam = SDLK_EXCLAIM, QuoteDbl = SDLK_QUOTEDBL,
    NumberSign = SDLK_LESS,
    Dollar = SDLK_DOLLAR, Percent = SDLK_PERCENT,
    Ampersand = SDLK_AMPERSAND,
    ParenLeft = SDLK_LEFTPAREN, ParenRight = SDLK_RIGHTPAREN,
    Asterisk = SDLK_ASTERISK, Plus = SDLK_PLUS, Comma = SDLK_COMMA,
    Minus = SDLK_MINUS, Period = SDLK_PERIOD, Slash = SDLK_SLASH,
    Num0 = SDLK_0, Num1 = SDLK_1, Num2 = SDLK_2, Num3 = SDLK_3,
    Num4 = SDLK_4, Num5 = SDLK_5, Num6 = SDLK_6, Num7 = SDLK_7,
    Num8 = SDLK_8, Num9 = SDLK_9, Colon = SDLK_COLON,
    Semicolon = SDLK_SEMICOLON,
    Less = SDLK_LESS, Equal = SDLK_EQUALS, Greater = SDLK_GREATER,
    Question = SDLK_QUESTION,
    At = SDLK_AT, A = SDLK_a, B = SDLK_b, C = SDLK_c, D = SDLK_d, E = SDLK_e,
    F = SDLK_f, G = SDLK_g, H = SDLK_h, I = SDLK_i, J = SDLK_j, K = SDLK_k,
    L = SDLK_l, M = SDLK_m, N = SDLK_n, O = SDLK_o, P = SDLK_p, Q = SDLK_q,
    R = SDLK_r, S = SDLK_s, T = SDLK_t, U = SDLK_u, V = SDLK_v, W = SDLK_w,
    X = SDLK_x, Y = SDLK_y, Z = SDLK_z,
    LBracket = SDLK_LEFTBRACKET, Backslash = SDLK_BACKSLASH,
    RBracket = SDLK_RIGHTBRACKET,
    Underscore = SDLK_UNDERSCORE, QuoteLeft = SDLK_QUOTE,
    LParen = SDLK_LEFTPAREN, RParen = SDLK_RIGHTPAREN,
    BackSpace = SDLK_BACKSPACE
};

enum class MouseButton
{
    None             = 0,
    Left             = SDL_BUTTON_LEFT,
    Right            = SDL_BUTTON_RIGHT,
    Middle           = SDL_BUTTON_MIDDLE,
    XButton1         = SDL_BUTTON_X1,
    XButton2         = SDL_BUTTON_X2
};

class Input
{
public:
    static String KeyToString(Key k);

    static bool GetKey(Key k);
    static bool GetKeyUp(Key k);
    static bool GetKeyDown(Key k);
    static bool GetKeyDownRepeat(Key k);

    static const Array<Key>& GetKeysUp();
    static const Array<Key>& GetKeysDown();
    static const Array<Key>& GetPressedKeys();

    static Vector2 GetMouseWheel();

    static Array<MouseButton> GetMouseButtons();
    static Array<MouseButton> GetMouseButtonsUp();
    static Array<MouseButton> GetMouseButtonsDown();
    static bool GetMouseButton(MouseButton mb);
    static bool GetMouseButtonUp(MouseButton mb);
    static bool GetMouseButtonDown(MouseButton mb);
    static bool GetMouseButtonDoubleClick(MouseButton mb);

    static bool IsMouseInsideScreen();

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

    static Vector2i GetMouseCoords();
    static Vector2 GetMouseCoordsNDC();
    static Vector2i GetPreviousMouseCoords();

    static void StartTextInput();
    static String PollInputText();
    static void StopTextInput();

    void Reset();

private:
    static constexpr float DoubleClickMaxSeconds = 0.25f;

    bool m_isADoubleClick          = false;
    bool m_lockMouseMovement       = false;
    bool m_isMouseInside           = false;
    float m_lastMouseDownTimestamp = 0;
    Vector2 m_lastMouseWheelDelta  = Vector2::Zero;

    String m_inputText = "";

    Vector2i m_lastMouseCoords = Vector2i::Zero;
    Vector2i m_lastClickMouseCoords = Vector2i::Zero;

    Array<Key> m_keysUp;
    Array<Key> m_keysDown;
    Array<Key> m_pressedKeys;

    FORWARD struct EventInfo;
    FORWARD struct ButtonInfo;
    Map<Key, ButtonInfo> m_keyInfos;
    Map<MouseButton, ButtonInfo> m_mouseInfo;
    Array<EventInfo> m_eventInfoQueue;

    Input();
    virtual ~Input();

    struct EventInfo : public IToString
    {
        enum Type
        {
            None,
            KeyDown, KeyUp,
            MouseDown, MouseUp, MouseMove,
            Wheel
        };

        Type type               = Type::None;
        Key key                 = Key::None;
        MouseButton mouseButton = MouseButton::None;
        bool autoRepeat         = false;
        int x                   = 0;
        int y                   = 0;
        float timestampSecs     = 0;
        Vector2 wheelDelta      = Vector2::Zero;

        String ToString() const override { return String::ToString(type); }
    };


    /**
     * @brief Struct to keep track of a Button state.
     * A button can either be a key or a mouse button,
     * and it contains the states up, down and pressed.
     */
    class ButtonInfo : public IToString
    {
        public:
            bool up = false;         // Just one frame
            bool down = false;       // Just one frame
            bool pressed = false;    // Long duration
            bool autoRepeat = false;

            ButtonInfo() { up = down = pressed = false; }
            ButtonInfo(bool up, bool down, bool pressed)
            {
                this->up = up;
                this->down = down;
                this->pressed = pressed;
            }

            String ToString() const override
            {
                std::ostringstream oss;
                oss << "(Up: " << up << ", Down: " << down <<
                       ", Pressed: " << pressed << ")";
                return String(oss.str());
            }
    };

    static Input* GetInstance();

    void ProcessMouseWheelEventInfo(const EventInfo &ei);
    void ProcessMouseMoveEventInfo(const EventInfo &ei);
    void ProcessMouseDownEventInfo(const EventInfo &ei);
    void ProcessMouseUpEventInfo(const EventInfo &ei);
    void ProcessKeyDownEventInfo(const EventInfo &ei);
    void ProcessKeyUpEventInfo(const EventInfo &ei);

    void PeekEvent(const SDL_Event &event, const Window *window);
    void EnqueueEvent(const EventInfo &eventInfo);
    void ProcessEventInfo(const EventInfo &eventInfo);

    void ProcessEnqueuedEvents();
    void OnFrameFinished();

    friend class Window;
};

NAMESPACE_BANG_END

#endif // INPUT_H
