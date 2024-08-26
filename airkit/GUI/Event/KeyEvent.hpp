#ifndef __KEYEVENT_H__
#define __KEYEVENT_H__

#include <airkit/GUI/Event/IEvent.hpp>

namespace airkit
{
    enum class KeyAction : uint8_t
    {
        Unknown,
        Down,   // 按下
        Up,     // 抬起
        Repeat, // 重复:不在事件中使用
    };

    // 按键修改符状态
    struct KeyMods
    {
        KeyMods(uint8_t value = 0)
            : value(value) {}

        KeyMods(const KeyMods &other)
            : value(other.value) {}

        KeyMods &operator=(uint8_t value)
        {
            this->value = value;
            return *this;
        }
        KeyMods &operator=(const KeyMods &other)
        {
            this->value = other.value;
            return *this;
        }
        union
        {
            uint8_t value;
            struct
            {
                uint8_t shift : 1;    // shift键
                uint8_t control : 1;  // ctrl键
                uint8_t alt : 1;      // alt键
                uint8_t super : 1;    // win键
                uint8_t capsLock : 1; // capsLock键
                uint8_t numLock : 1;  // numLock键
            };
        };

        std::string toString() const
        {
            std::string ss;
            if (shift)
                ss += "Shift+";
            if (control)
                ss += "Control+";
            if (alt)
                ss += "Alt+";
            if (super)
                ss += "Super+";
            if (capsLock)
                ss += "CapsLock+";
            if (numLock)
                ss += "NumLock+";
            return ss;
        }
    };

    // 键盘按钮：按照美国标准
    enum class KeyButton : uint16_t
    {
        Unknown,
        // ----------------可打印字符按键-------------
        Space = 32,      // 空格键: ' '
        Apostrophe = 39, // 单引号键: '
        Comma = 44,      // 逗号键: ,
        Minus = 45,      // 减号键: -
        Period = 46,     // 点号键: .
        Slash = 47,      // 斜杠键: /

        D0 = 48, // 数字 0
        D1 = 49, // 数字 1
        D2 = 50, // 数字 2
        D3 = 51, // 数字 3
        D4 = 52, // 数字 4
        D5 = 53, // 数字 5
        D6 = 54, // 数字 6
        D7 = 55, // 数字 7
        D8 = 56, // 数字 8
        D9 = 57, // 数字9

        Semicolon = 59, // 分号键: ;
        Equal = 61,     // 等号键: =

        A = 65,
        B = 66,
        C = 67,
        D = 68,
        E = 69,
        F = 70,
        G = 71,
        H = 72,
        I = 73,
        J = 74,
        K = 75,
        L = 76,
        M = 77,
        N = 78,
        O = 79,
        P = 80,
        Q = 81,
        R = 82,
        S = 83,
        T = 84,
        U = 85,
        V = 86,
        W = 87,
        X = 88,
        Y = 89,
        Z = 90,

        LeftBracket = 91,  // 左方括号：[
        Backslash = 92,    // 反斜杠：\1
        RightBracket = 93, // 右方括号：]
        GraveAccent = 96,  // 重音符：`

        // 功能按键
        Escape = 256,      // 退出键：ESC
        Enter = 257,       // 回车键：Enter
        Tab = 258,         // 制表键：Tab
        Backspace = 259,   // 退格键：Backspace
        Insert = 260,      // 插入键：Insert
        Delete = 261,      // 删除键：Delete
        RightArrow = 262,  // 右箭头：Right ->
        LeftArrow = 263,   // 左箭头：Left <-
        DownArrow = 264,   // 下箭头：Down |v
        UpArrow = 265,     // 上箭头：Up ^|
        PageUp = 266,      // 页面上滚动键：PageUp
        PageDown = 267,    // 页面下滚动键：PageDown
        Home = 268,        // 首页键：Home
        End = 269,         // 末页键：End
        CapsLock = 280,    // 大写锁定键：CapsLock
        NumLock = 282,     // 数字锁定键：NumLock
        ScrollLock = 283,  // 滚动锁定键：ScrollLock
        PrintScreen = 284, // 截图键：PrintScreen
        Pause = 285,       // 暂停键：Pause

        // Fn 系列键
        F1 = 290,  // F1键：F1
        F2 = 291,  // F2键：F2
        F3 = 292,  // F3键：F3
        F4 = 293,  // F4键：F4
        F5 = 294,  // F5键：F5
        F6 = 295,  // F6键：F6
        F7 = 296,  // F7键：F7
        F8 = 297,  // F8键：F8
        F9 = 298,  // F9键：F9
        F10 = 299, // F10键：F10
        F11 = 300, // F11键：F11
        F12 = 301, // F12键：F12
        F13 = 302, // F13键：F13
        F14 = 303, // F14键：F14
        F15 = 304, // F15键：F15
        F16 = 305, // F16键：F16
        F17 = 306, // F17键：F17
        F18 = 307, // F18键：F18
        F19 = 308, // F19键：F19
        F20 = 309, // F20键：F20
        F21 = 310, // F21键：F21
        F22 = 311, // F22键：F22
        F23 = 312, // F23键：F23
        F24 = 313, // F24键：F24
                   // F25 = 314, // F25键：F25

        // Numpad 键
        NP_0 = 320,       // Numpad 0 键： 0
        NP_1 = 321,       // Numpad 1 键： 1
        NP_2 = 322,       // Numpad 2 键： 2
        NP_3 = 323,       // Numpad 3 键： 3
        NP_4 = 324,       // Numpad 4 键： 4
        NP_5 = 325,       // Numpad 5 键： 5
        NP_6 = 326,       // Numpad 6 键： 6
        NP_7 = 327,       // Numpad 7 键： 7
        NP_8 = 328,       // Numpad 8 键： 8
        NP_9 = 329,       // Numpad 9 键： 9
        NP_Decimal = 330, // Numpad . 键： .
        NP_Div = 331,     // Numpad / 键： /
        NP_Mul = 332,     // Numpad * 键： *
        NP_Sub = 333,     // Numpad - 键： -
        NP_Add = 334,     // Numpad + 键： +

        // NP_Enter = 335,    // Numpad Enter 键： Enter
        // NP_Equal = 336,    // Numpad = 键： =

        // 控制性键

        LeftShift = 340,    // 左 Shift 键： 左 Shift
        RightShift = 341,   // 右 Shift 键： 右 Shift
        LeftControl = 342,  // 左 Ctrl 键： 左 Ctrl
        RightControl = 343, // 右 Ctrl 键： 右 Ctrl
        LeftAlt = 344,      // 左 Alt 键： 左 Alt
        RightAlt = 345,     // 右 Alt 键： 右 Alt
        LeftSuper = 346,    // 左 Win 键：左 Win
        RightSuper = 347,   // 右 Win 键：右 Win
        Menu = 348,         // 菜单键：菜单键

        Shift = 350,   // Shift 键：Shift，不区分左右
        Control = 351, // Ctrl 键：Ctrl，不区分左右
        Alt = 352,     // Alt 键：Alt，不区分左右
        Super = 353,   // Win 键：Win，不区分左右

        // 媒体键

        MediaPlayPause = 400,  // 播放/暂停键
        MediaStop = 401,       // 停止键
        MediaNext = 402,       // 下一曲键
        MediaPrevious = 403,   // 上一曲键
        MediaVolumeUp = 404,   // 音量加键
        MediaVolumeDown = 405, // 音量减键
        MediaMute = 406,       // 静音键

    };

    const char *getKeyName(KeyButton key);
    struct IKeyEvent : public IEvent
    {

        virtual ~IKeyEvent() = default;
        KeyAction getAction() const { return mAction; }
        KeyButton getButton() const { return mButton; }

    protected:
        IKeyEvent(KeyAction action, KeyButton button)
            : IEvent(EventKind::Keyboard), mAction(action), mButton(button) {}

    private:
        KeyAction mAction;
        KeyButton mButton;
    };

    struct KeyDownEvent : public IKeyEvent
    {
        KeyDownEvent(KeyButton button, bool isRepeat, KeyMods mods)
            : IKeyEvent(KeyAction::Down, button),
              mIsRepeat(isRepeat), mMods(mods) {}
        virtual ~KeyDownEvent() = default;

        bool isRepeat() const { return mIsRepeat; }
        KeyMods getMods() const { return mMods; }

    protected:
        bool mIsRepeat; // 是否重复
        KeyMods mMods;
    };

    struct KeyUpEvent : public IKeyEvent
    {
        KeyUpEvent(KeyButton button)
            : IKeyEvent(KeyAction::Up, button) {}
        virtual ~KeyUpEvent() = default;
    };
}
#endif // __KEYEVENT_H__