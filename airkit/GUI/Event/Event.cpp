
#include <airkit/GUI/Event/Event.hpp>

namespace airkit
{
    IEvent::~IEvent() {}
    const char *getKeyName(KeyButton key)
    {
        switch (key)
        {
        case KeyButton::Space:
            return "Space";
        case KeyButton::Apostrophe:
            return "'";
        case KeyButton::Comma:
            return ",";
        case KeyButton::Minus:
            return "-";
        case KeyButton::Period:
            return ".";
        case KeyButton::Slash:
            return "/";

        case KeyButton::D0:
            return "0";
        case KeyButton::D1:
            return "1";
        case KeyButton::D2:
            return "2";
        case KeyButton::D3:
            return "3";
        case KeyButton::D4:
            return "4";
        case KeyButton::D5:
            return "5";
        case KeyButton::D6:
            return "6";
        case KeyButton::D7:
            return "7";
        case KeyButton::D8:
            return "8";
        case KeyButton::D9:
            return "9";

        case KeyButton::Semicolon:
            return ";";
        case KeyButton::Equal:
            return "=";

        case KeyButton::A:
            return "A";
        case KeyButton::B:
            return "B";
        case KeyButton::C:
            return "C";
        case KeyButton::D:
            return "D";
        case KeyButton::E:
            return "E";
        case KeyButton::F:
            return "F";
        case KeyButton::G:
            return "G";
        case KeyButton::H:
            return "H";
        case KeyButton::I:
            return "I";
        case KeyButton::J:
            return "J";
        case KeyButton::K:
            return "K";
        case KeyButton::L:
            return "L";
        case KeyButton::M:
            return "M";
        case KeyButton::N:
            return "N";
        case KeyButton::O:
            return "O";
        case KeyButton::P:
            return "P";
        case KeyButton::Q:
            return "Q";
        case KeyButton::R:
            return "R";
        case KeyButton::S:
            return "S";
        case KeyButton::T:
            return "T";
        case KeyButton::U:
            return "U";
        case KeyButton::V:
            return "V";
        case KeyButton::W:
            return "W";
        case KeyButton::X:
            return "X";
        case KeyButton::Y:
            return "Y";
        case KeyButton::Z:
            return "Z";

        case KeyButton::LeftBracket:
            return "[";
        case KeyButton::Backslash:
            return "\\";
        case KeyButton::RightBracket:
            return "]";
        case KeyButton::GraveAccent:
            return "`";

        case KeyButton::Escape:
            return "Esc";
        case KeyButton::Enter:
            return "Enter";
        case KeyButton::Tab:
            return "Tab";
        case KeyButton::Backspace:
            return "Backspace";
        case KeyButton::Insert:
            return "Insert";
        case KeyButton::Delete:
            return "Delete";
        case KeyButton::RightArrow:
            return "RightArrow";
        case KeyButton::LeftArrow:
            return "LeftArrow";
        case KeyButton::DownArrow:
            return "DownArrow";
        case KeyButton::UpArrow:
            return "UpArrow";
        case KeyButton::PageUp:
            return "PageUp";
        case KeyButton::PageDown:
            return "PageDown";
        case KeyButton::Home:
            return "Home";
        case KeyButton::End:
            return "End";
        case KeyButton::CapsLock:
            return "CapsLock";
        case KeyButton::ScrollLock:
            return "ScrollLock";
        case KeyButton::NumLock:
            return "NumLock";
        case KeyButton::PrintScreen:
            return "PrintScreen";
        case KeyButton::Pause:
            return "Pause";

        case KeyButton::F1:
            return "F1";
        case KeyButton::F2:
            return "F2";
        case KeyButton::F3:
            return "F3";
        case KeyButton::F4:
            return "F4";
        case KeyButton::F5:
            return "F5";
        case KeyButton::F6:
            return "F6";
        case KeyButton::F7:
            return "F7";
        case KeyButton::F8:
            return "F8";
        case KeyButton::F9:
            return "F9";
        case KeyButton::F10:
            return "F10";
        case KeyButton::F11:
            return "F11";
        case KeyButton::F12:
            return "F12";
        case KeyButton::F13:
            return "F13";
        case KeyButton::F14:
            return "F14";
        case KeyButton::F15:
            return "F15";
        case KeyButton::F16:
            return "F16";
        case KeyButton::F17:
            return "F17";
        case KeyButton::F18:
            return "F18";
        case KeyButton::F19:
            return "F19";
        case KeyButton::F20:
            return "F20";
        case KeyButton::F21:
            return "F21";
        case KeyButton::F22:
            return "F22";
        case KeyButton::F23:
            return "F23";
        case KeyButton::F24:
            return "F24";

        case KeyButton::NP_0:
            return "0";
        case KeyButton::NP_1:
            return "1";
        case KeyButton::NP_2:
            return "2";
        case KeyButton::NP_3:
            return "3";
        case KeyButton::NP_4:
            return "4";
        case KeyButton::NP_5:
            return "5";
        case KeyButton::NP_6:
            return "6";
        case KeyButton::NP_7:
            return "7";
        case KeyButton::NP_8:
            return "8";
        case KeyButton::NP_9:
            return "9";
        case KeyButton::NP_Decimal:
            return ".";
        case KeyButton::NP_Div:
            return "/";
        case KeyButton::NP_Mul:
            return "*";
        case KeyButton::NP_Sub:
            return "-";
        case KeyButton::NP_Add:
            return "+";

        case KeyButton::Shift:
        case KeyButton::LeftShift:
        case KeyButton::RightShift:
            return "Shift";

        case KeyButton::Control:
        case KeyButton::LeftControl:
        case KeyButton::RightControl:
            return "Control";

        case KeyButton::Alt:
        case KeyButton::LeftAlt:
        case KeyButton::RightAlt:
            return "Alt";

        case KeyButton::Super:
        case KeyButton::LeftSuper:
        case KeyButton::RightSuper:
            return "Super";

        case KeyButton::Menu:
            return "Menu";

        case KeyButton::MediaPlayPause:
            return "MediaPlayPause";
        case KeyButton::MediaStop:
            return "MediaStop";
        case KeyButton::MediaPrevious:
            return "MediaPrev";
        case KeyButton::MediaNext:
            return "MediaNext";

        case KeyButton::MediaMute:
            return "VolumeMute";
        case KeyButton::MediaVolumeUp:
            return "VolumeUp";
        case KeyButton::MediaVolumeDown:
            return "VolumeDown";
        }
        return nullptr;
    }

}