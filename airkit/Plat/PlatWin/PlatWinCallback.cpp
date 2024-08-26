#include <airkit/Plat/PlatWin/PlatWin.hpp>
#include <airkit/Plat/PlatWin/GLWindow.hpp>
#include <airkit/Render/GL/GLRender.hpp>
#define FMT_HEADER_ONLY
#include <airkit/3Part/fmt/core.h>
#include <airkit/3Part/fmt/color.h>

#include <windowsx.h>
#include <uxtheme.h>
#include <vssym32.h>
#include "PlatWin.hpp"

namespace airkit
{

    template <typename T>
    inline void mouse_event_btn(HWND handle, MouseButton btn, LPARAM l_param)
    {
        auto userdata = GetWindowLongPtr(handle, GWLP_USERDATA);
        if (userdata != 0)
        {
            WinWindow &win = *(WinWindow *)userdata;

            auto cursor = win.getCursorPos();
            T ev(btn, cursor);
            win.onEvent(ev);
        }
    }
    LRESULT PlatWin::wincallback(HWND handle, UINT message, WPARAM w_param, LPARAM l_param)
    {
        switch (message)
        {

        case WM_NCCALCSIZE:
            return onWM_NCCALCSIZE(handle, message, w_param, l_param);
        case WM_NCHITTEST:
            return onWM_NCHITTEST(handle, message, w_param, l_param);
        case WM_CREATE:
        {
            RECT size_rect;
            GetWindowRect(handle, &size_rect);

            // Inform the application of the frame change to force redrawing with the new
            // client area that is extended into the title bar
            auto width = size_rect.right - size_rect.left;
            auto height = size_rect.bottom - size_rect.top;

            SetWindowPos(
                handle, NULL,
                size_rect.left, size_rect.top,
                width, height,
                SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
            break;
        }
        case WM_SETCURSOR:
        {
            SetCursor(LoadCursor(NULL, IDC_ARROW));
            auto userdata = GetWindowLongPtr(handle, GWLP_USERDATA);
            if (userdata != 0)
            {
                WinWindow &win = *(WinWindow *)userdata;
            }
            else
                // Show an arrow instead of the busy cursor
                SetCursor(LoadCursor(NULL, IDC_ARROW));
            break;
        }
        case WM_CLOSE:
        {
            auto userdata = GetWindowLongPtr(handle, GWLP_USERDATA);
            if (userdata != 0)
            {
                WinWindow &win = *(WinWindow *)userdata;
                if (true == win.onClose())
                {
                    win.setShouldClose(true);
                    DestroyWindow(handle);
                }
            }
            return 0;
        }
        break;
        case WM_DESTROY:
        {
            auto userdata = GetWindowLongPtr(handle, GWLP_USERDATA);
            if (userdata != 0)
            {
                WinWindow &win = *(WinWindow *)userdata;
                // 窗口销毁应当从管理器中删除
                IPlat::getInstance().releaseWindow(&win);
                return 0;
            }
        }
        break;

        // 限制窗口大小
        case WM_GETMINMAXINFO:
        {
            auto userdata = GetWindowLongPtr(handle, GWLP_USERDATA);
            if (userdata != 0)
            {
                WinWindow &win = *(WinWindow *)userdata;

                auto limit = win.getUILimit();
                auto &h = limit.getH();
                auto &v = limit.getV();
                auto info = (MINMAXINFO *)l_param;

                info->ptMinTrackSize.x = h.getMin();
                auto bx = info->ptMaxTrackSize.x;
                info->ptMaxTrackSize.x = h.getMax();
                if (info->ptMaxTrackSize.x < 0)
                    info->ptMaxTrackSize.x = bx;

                info->ptMinTrackSize.y = v.getMin();
                auto by = info->ptMaxTrackSize.y;
                info->ptMaxTrackSize.y = v.getMax();
                if (info->ptMaxTrackSize.y < 0)
                    info->ptMaxTrackSize.y = by;

                return 0;
            }
        }
        break;

        case WM_WINDOWPOSCHANGING:
        {
            auto userdata = GetWindowLongPtr(handle, GWLP_USERDATA);
            if (userdata != 0)
            {
                WinWindow &win = *(WinWindow *)userdata;
                auto pos = (WINDOWPOS *)l_param;

                auto flag = pos->flags;

                auto w = pos->cx;
                auto h = pos->cy;
                auto x = pos->x;
                auto y = pos->y;

                // 判断大小是否有变化
                auto &area = win.mArea;
                if (flag & SWP_NOSIZE == false) //(area.getWidth() != w || area.getHeight() != h)
                {
                    // 生成大小变化事件
                    UIArea na(x, y, w, h);
                    UIResizingEvent ev(na);
                    win.onEvent(ev);
                }
                // 判断位置是否有变化
                if (flag & SWP_NOMOVE == false) //(area.getPos().getX() != x || area.getPos().getY() != y)
                {
                    // 生成位置变化事件
                    UIMovingEvent ev(x, y);
                    win.onEvent(ev);
                }
                return 0;
            }
        }
        break;
        case WM_WINDOWPOSCHANGED:
        {
            auto userdata = GetWindowLongPtr(handle, GWLP_USERDATA);
            if (userdata != 0)
            {
                WinWindow &win = *(WinWindow *)userdata;
                auto pos = (WINDOWPOS *)l_param;

                auto flag = pos->flags;

                auto w = pos->cx;
                auto h = pos->cy;
                auto x = pos->x;
                auto y = pos->y;

                // 判断大小是否有变化
                auto &area = win.mArea;
                if (area.getWidth() != w || area.getHeight() != h)
                {
                    // 生成大小变化事件
                    UIArea na(x, y, w, h);
                    UIResizedEvent ev(na);
                    win.onEvent(ev);
                }
                // 判断位置是否有变化
                if (area.getPos().getX() != x || area.getPos().getY() != y)
                {
                    // 生成位置变化事件
                    UIMovedEvent ev(x, y);
                    win.onEvent(ev);
                }
                return 0;
            }
        }
        break;

        case WM_SYSCOMMAND:
        {
            auto userdata = GetWindowLongPtr(handle, GWLP_USERDATA);
            if (userdata != 0)
            {
                WinWindow &win = *(WinWindow *)userdata;
                // 当窗口处于全屏时，禁止进入系统保护模式
                auto flag = w_param & 0xfff0;
                if (win.isFullScreen() &&
                    (SC_SCREENSAVE == flag ||
                     SC_MONITORPOWER == flag))
                    return 0;
            }
        }
        break;
        case WM_CHAR:
        case WM_SYSCHAR:
        {
            auto userdata = GetWindowLongPtr(handle, GWLP_USERDATA);
            if (userdata != 0)
            {
                WinWindow &win = *(WinWindow *)userdata;
                if (w_param >= 0xd800 && w_param <= 0xdbff)
                    win.setHighSurrogate(w_param);
                else
                {
                    uint32_t codepoint = 0;

                    if (w_param >= 0xdc00 && w_param <= 0xdfff)
                    {
                        auto high = win.getHighSurrogate();
                        if (high)
                        {
                            codepoint += (high - 0xd800) << 10;
                            codepoint += (WCHAR)w_param - 0xdc00;
                            codepoint += 0x10000;
                        }
                    }
                    else
                        codepoint = (WCHAR)w_param;

                    win.setHighSurrogate(0);
                    CharInputEvent ev(codepoint);
                    win.onEvent(ev);
                }
            }
            return 0;
        }
        break;
        case WM_UNICHAR:
        {
            // 测试是否支持UNICODE
            if (w_param == UNICODE_NOCHAR)
                return TRUE;
            auto userdata = GetWindowLongPtr(handle, GWLP_USERDATA);
            if (userdata != 0)
            {
                WinWindow &win = *(WinWindow *)userdata;
                CharInputEvent ev(w_param);
                win.onEvent(ev);
            }
            return 0;
        }

        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        {
            auto userdata = GetWindowLongPtr(handle, GWLP_USERDATA);
            if (userdata != 0)
            {
                WinWindow &win = *(WinWindow *)userdata;
                auto key = translateKey(w_param);
                if (key == KeyButton::Unknown)
                    return 0;
                auto mods = getKeyMods();
                bool repeat = (l_param & 0x40000000) != 0;
                KeyDownEvent ev(key, repeat, mods);
                win.onEvent(ev);
                return 0;
            }
        }
        break;
        case WM_KEYUP:
        case WM_SYSKEYUP:
        {
            auto userdata = GetWindowLongPtr(handle, GWLP_USERDATA);
            if (userdata != 0)
            {
                WinWindow &win = *(WinWindow *)userdata;
                auto key = translateKey(w_param);
                auto ev = KeyUpEvent(key);
                win.onEvent(ev);
                return 0;
            }
        }
        break;
        case WM_NCMOUSEMOVE:
        case WM_MOUSEMOVE:
        {
            auto userdata = GetWindowLongPtr(handle, GWLP_USERDATA);
            if (userdata != 0)
            {
                WinWindow &win = *(WinWindow *)userdata;
                auto cursor = win.getCursorPos();

                // 判断鼠标是否进入窗口
                if (0 == win.getUIFlag(UIFlag::MouseEnter))
                {
                    win.setUIFlag(UIFlag::MouseEnter);
                    MouseEnterEvent ev(cursor);
                    win.onEvent(ev);
                }

                // 判断鼠标是否在窗口内

                MouseMoveEvent ev(cursor);
                win.onEvent(ev);
            }
        }
        break;

        case WM_NCMOUSELEAVE:
        case WM_MOUSELEAVE:
        {
            auto userdata = GetWindowLongPtr(handle, GWLP_USERDATA);
            if (userdata != 0)
            {
                WinWindow &win = *(WinWindow *)userdata;
                auto cursor = win.getCursorPos();
                win.resetUIFlag(UIFlag::MouseEnter);
                MouseLeaveEvent ev(cursor);
                win.onEvent(ev);
                return 0;
            }
        }
        break;

        case WM_MOUSEWHEEL:
        {
            auto userdata = GetWindowLongPtr(handle, GWLP_USERDATA);
            if (userdata != 0)
            {
                WinWindow &win = *(WinWindow *)userdata;
                auto cursor = win.getCursorPos();

                auto z = GET_WHEEL_DELTA_WPARAM(w_param);
                MouseWheelEvent ev(cursor, float(z) / float(WHEEL_DELTA));
                win.onEvent(ev);
                return 0;
            }
        }
        break;
        case WM_NCLBUTTONDOWN:
        case WM_LBUTTONDOWN:
        {
            // mouse_event_btn<MouseDownEvent>(handle, MouseButton::Left, l_param);
            auto userdata = GetWindowLongPtr(handle, GWLP_USERDATA);
            if (userdata != 0)
            {
                WinWindow &win = *(WinWindow *)userdata;

                auto cursor = win.getCursorPos();
                MouseDownEvent ev(MouseButton::Left, cursor);
                win.onEvent(ev);
                auto hit = win.onWinHitTest(cursor);
                switch (hit)
                {
                case HTCLOSE:
                    return 0;
                case HTMINBUTTON:
                    return 0;
                case HTMAXBUTTON:
                    return 0;
                }
            }
        }
        break;
        case WM_NCRBUTTONDOWN:
        case WM_RBUTTONDOWN:
            mouse_event_btn<MouseDownEvent>(handle, MouseButton::Right, l_param);
            break;
        case WM_NCMBUTTONDOWN:
        case WM_MBUTTONDOWN:
            mouse_event_btn<MouseDownEvent>(handle, MouseButton::Middle, l_param);
            break;

        case WM_NCLBUTTONUP:
        case WM_LBUTTONUP:
            mouse_event_btn<MouseUpEvent>(handle, MouseButton::Left, l_param);
            break;
        case WM_NCRBUTTONUP:
        case WM_RBUTTONUP:
            mouse_event_btn<MouseUpEvent>(handle, MouseButton::Right, l_param);

            break;
        case WM_NCMBUTTONUP:
        case WM_MBUTTONUP:
            mouse_event_btn<MouseUpEvent>(handle, MouseButton::Middle, l_param);
            if (message == WM_NCMBUTTONUP)
                return 0;
            break;

        case WM_ACTIVATE:
        {
            auto userdata = GetWindowLongPtr(handle, GWLP_USERDATA);
            if (userdata != 0)
            {
                WinWindow &win = *(WinWindow *)userdata;
                auto cursor = win.getCursorPos();
                if (w_param == WA_INACTIVE)
                {
                    win.resetUIFlag(UIFlag::MouseEnter);
                    MouseLeaveEvent ev(cursor);
                }
                // return 0;
            }
        }
        }
        return DefWindowProcA(handle, message, w_param, l_param);
    }

    // Handling this event allows us to extend client (paintable) area into the title bar region
    // The information is partially coming from:
    // https://docs.microsoft.com/en-us/windows/win32/dwm/customframe#extending-the-client-frame
    // Most important paragraph is:
    //   To remove the standard window frame, you must handle the WM_NCCALCSIZE message,
    //   specifically when its wParam value is TRUE and the return value is 0.
    //   By doing so, your application uses the entire window region as the client area,
    //   removing the standard frame.
    LRESULT PlatWin::onWM_NCCALCSIZE(HWND handle, UINT message, WPARAM w_param, LPARAM l_param)
    {
        if (!w_param)
            return DefWindowProcA(handle, message, w_param, l_param);

        UINT dpi = GetDpiForWindow(handle);

        int frame_x = GetSystemMetricsForDpi(SM_CXFRAME, dpi);
        int frame_y = GetSystemMetricsForDpi(SM_CYFRAME, dpi);
        int padding = GetSystemMetricsForDpi(SM_CXPADDEDBORDER, dpi);

        NCCALCSIZE_PARAMS *params = (NCCALCSIZE_PARAMS *)l_param;
        RECT *requested_client_rect = params->rgrc;

        requested_client_rect->right -= frame_x + padding;
        requested_client_rect->left += frame_x + padding;
        requested_client_rect->bottom -= frame_y + padding;

        // 检查窗口是否最大化
        WINDOWPLACEMENT placement = {0};
        placement.length = sizeof(WINDOWPLACEMENT);
        if (GetWindowPlacement(handle, &placement) &&
            placement.showCmd == SW_SHOWMAXIMIZED)
            requested_client_rect->top += padding;

        return 0;
    }

    LRESULT PlatWin::onWM_NCHITTEST(HWND handle, UINT message, WPARAM w_param, LPARAM l_param)
    {
        LRESULT hit = DefWindowProc(handle, message, w_param, l_param);
        switch (hit)
        {
        case HTNOWHERE:
        case HTRIGHT:
        case HTLEFT:
        case HTTOPLEFT:
        case HTTOP:
        case HTTOPRIGHT:
        case HTBOTTOMRIGHT:
        case HTBOTTOM:
        case HTBOTTOMLEFT:
            return hit;
        }
        auto userdata = GetWindowLongPtr(handle, GWLP_USERDATA);
        if (userdata == 0)
            return HTCLIENT;

        WinWindow &win = *(WinWindow *)userdata;
        auto cursor = win.getCursorPos();

        // UIPoint cursor(cursor_point.x, cursor_point.y);

        return win.onWinHitTest(cursor);
    }

    KeyButton PlatWin::translateKey(WPARAM w_param)
    {
        switch (w_param)
        {
        case VK_SPACE:
            return KeyButton::Space;
        case VK_OEM_7:
            return KeyButton::Apostrophe;
        case VK_OEM_COMMA:
            return KeyButton::Comma;
        case VK_OEM_MINUS:
            return KeyButton::Minus;
        case VK_OEM_PERIOD:
            return KeyButton::Period;
        case VK_OEM_2:
            return KeyButton::Slash;

        // 数字键：0~9
        case 0x30:
            return KeyButton::D0;
        case 0x31:
            return KeyButton::D1;
        case 0x32:
            return KeyButton::D2;
        case 0x33:
            return KeyButton::D3;
        case 0x34:
            return KeyButton::D4;
        case 0x35:
            return KeyButton::D5;
        case 0x36:
            return KeyButton::D6;
        case 0x37:
            return KeyButton::D7;
        case 0x38:
            return KeyButton::D8;
        case 0x39:
            return KeyButton::D9;

        case VK_OEM_1:
            return KeyButton::Semicolon;
        case VK_OEM_PLUS:
            return KeyButton::Equal;

        // 字母键：A~Z
        case 0x41:
            return KeyButton::A;
        case 0x42:
            return KeyButton::B;
        case 0x43:
            return KeyButton::C;
        case 0x44:
            return KeyButton::D;
        case 0x45:
            return KeyButton::E;
        case 0x46:
            return KeyButton::F;
        case 0x47:
            return KeyButton::G;
        case 0x48:
            return KeyButton::H;
        case 0x49:
            return KeyButton::I;
        case 0x4A:
            return KeyButton::J;
        case 0x4B:
            return KeyButton::K;
        case 0x4C:
            return KeyButton::L;
        case 0x4D:
            return KeyButton::M;
        case 0x4E:
            return KeyButton::N;
        case 0x4F:
            return KeyButton::O;
        case 0x50:
            return KeyButton::P;
        case 0x51:
            return KeyButton::Q;
        case 0x52:
            return KeyButton::R;
        case 0x53:
            return KeyButton::S;
        case 0x54:
            return KeyButton::T;
        case 0x55:
            return KeyButton::U;
        case 0x56:
            return KeyButton::V;
        case 0x57:
            return KeyButton::W;
        case 0x58:
            return KeyButton::X;
        case 0x59:
            return KeyButton::Y;
        case 0x5A:
            return KeyButton::Z;

        case VK_OEM_4:
            return KeyButton::LeftBracket;
        case VK_OEM_5:
            return KeyButton::Backslash;
        case VK_OEM_6:
            return KeyButton::RightBracket;
        case VK_OEM_3:
            return KeyButton::GraveAccent;

        // 功能按键
        case VK_ESCAPE:
            return KeyButton::Escape;
        case VK_RETURN:
            return KeyButton::Enter;
        case VK_TAB:
            return KeyButton::Tab;
        case VK_BACK:
            return KeyButton::Backspace;
        case VK_INSERT:
            return KeyButton::Insert;
        case VK_DELETE:
            return KeyButton::Delete;
        case VK_LEFT:
            return KeyButton::LeftArrow;
        case VK_RIGHT:
            return KeyButton::RightArrow;
        case VK_UP:
            return KeyButton::UpArrow;
        case VK_DOWN:
            return KeyButton::DownArrow;
        case VK_PRIOR:
            return KeyButton::PageUp;
        case VK_NEXT:
            return KeyButton::PageDown;
        case VK_HOME:
            return KeyButton::Home;
        case VK_END:
            return KeyButton::End;

        case VK_CAPITAL:
            return KeyButton::CapsLock;
        case VK_NUMLOCK:
            return KeyButton::NumLock;
        case VK_SCROLL:
            return KeyButton::ScrollLock;
        case VK_SNAPSHOT:
            return KeyButton::PrintScreen;
        case VK_PAUSE:
            return KeyButton::Pause;

        // Fn 系列键
        case VK_F1:
            return KeyButton::F1;
        case VK_F2:
            return KeyButton::F2;
        case VK_F3:
            return KeyButton::F3;
        case VK_F4:
            return KeyButton::F4;
        case VK_F5:
            return KeyButton::F5;
        case VK_F6:
            return KeyButton::F6;
        case VK_F7:
            return KeyButton::F7;
        case VK_F8:
            return KeyButton::F8;
        case VK_F9:
            return KeyButton::F9;
        case VK_F10:
            return KeyButton::F10;
        case VK_F11:
            return KeyButton::F11;
        case VK_F12:
            return KeyButton::F12;
        case VK_F13:
            return KeyButton::F13;
        case VK_F14:
            return KeyButton::F14;
        case VK_F15:
            return KeyButton::F15;
        case VK_F16:
            return KeyButton::F16;
        case VK_F17:
            return KeyButton::F17;
        case VK_F18:
            return KeyButton::F18;
        case VK_F19:
            return KeyButton::F19;
        case VK_F20:
            return KeyButton::F20;
        case VK_F21:
            return KeyButton::F21;
        case VK_F22:
            return KeyButton::F22;
        case VK_F23:
            return KeyButton::F23;
        case VK_F24:
            return KeyButton::F24;

            // 小键盘数字键
        case VK_NUMPAD0:
            return KeyButton::NP_0;
        case VK_NUMPAD1:
            return KeyButton::NP_1;
        case VK_NUMPAD2:
            return KeyButton::NP_2;
        case VK_NUMPAD3:
            return KeyButton::NP_3;
        case VK_NUMPAD4:
            return KeyButton::NP_4;
        case VK_NUMPAD5:
            return KeyButton::NP_5;
        case VK_NUMPAD6:
            return KeyButton::NP_6;
        case VK_NUMPAD7:
            return KeyButton::NP_7;
        case VK_NUMPAD8:
            return KeyButton::NP_8;
        case VK_NUMPAD9:
            return KeyButton::NP_9;
        case VK_DECIMAL:
            return KeyButton::NP_Decimal;
        case VK_DIVIDE:
            return KeyButton::NP_Div;
        case VK_MULTIPLY:
            return KeyButton::NP_Mul;
        case VK_SUBTRACT:
            return KeyButton::NP_Sub;
        case VK_ADD:
            return KeyButton::NP_Add;

        case VK_SHIFT:
        case VK_LSHIFT:
        case VK_RSHIFT:
            return KeyButton::Shift;

        case VK_CONTROL:
        case VK_LCONTROL:
        case VK_RCONTROL:
            return KeyButton::Control;
        case VK_MENU:
        case VK_LMENU:
        case VK_RMENU:
            return KeyButton::Alt;

        case VK_LWIN:
        case VK_RWIN:
            return KeyButton::Super;

        case VK_APPS:
            return KeyButton::Menu;

        case VK_MEDIA_PLAY_PAUSE:
            return KeyButton::MediaPlayPause;
        case VK_MEDIA_STOP:
            return KeyButton::MediaStop;
        case VK_MEDIA_PREV_TRACK:
            return KeyButton::MediaPrevious;
        case VK_MEDIA_NEXT_TRACK:
            return KeyButton::MediaNext;
        case VK_VOLUME_MUTE:
            return KeyButton::MediaMute;
        case VK_VOLUME_DOWN:
            return KeyButton::MediaVolumeDown;
        case VK_VOLUME_UP:
            return KeyButton::MediaVolumeUp;
        }
        return KeyButton::Unknown;
    }

    uint32_t PlatWin::translateKey(KeyButton key)
    {
        switch (key)
        {
        case KeyButton::Space:
            return VK_SPACE;
        case KeyButton::Apostrophe:
            return VK_OEM_7;
        case KeyButton::Comma:
            return VK_OEM_COMMA;
        case KeyButton::Minus:
            return VK_OEM_MINUS;
        case KeyButton::Period:
            return VK_OEM_PERIOD;
        case KeyButton::Slash:
            return VK_OEM_2;

        // 数字键：0~9
        case KeyButton::D0:
            return 0x30;
        case KeyButton::D1:
            return 0x31;
        case KeyButton::D2:
            return 0x32;
        case KeyButton::D3:
            return 0x33;
        case KeyButton::D4:
            return 0x34;
        case KeyButton::D5:
            return 0x35;
        case KeyButton::D6:
            return 0x36;
        case KeyButton::D7:
            return 0x37;
        case KeyButton::D8:
            return 0x38;
        case KeyButton::D9:
            return 0x39;

        case KeyButton::Semicolon:
            return VK_OEM_1;
        case KeyButton::Equal:
            return VK_OEM_PLUS;

        // 字母键：A~Z
        case (KeyButton)0x41:
            return (uint32_t)KeyButton::A;
        case (KeyButton)0x42:
            return (uint32_t)KeyButton::B;
        case (KeyButton)0x43:
            return (uint32_t)KeyButton::C;
        case (KeyButton)0x44:
            return (uint32_t)KeyButton::D;
        case (KeyButton)0x45:
            return (uint32_t)KeyButton::E;
        case (KeyButton)0x46:
            return (uint32_t)KeyButton::F;
        case (KeyButton)0x47:
            return (uint32_t)KeyButton::G;
        case (KeyButton)0x48:
            return (uint32_t)KeyButton::H;
        case (KeyButton)0x49:
            return (uint32_t)KeyButton::I;
        case (KeyButton)0x4A:
            return (uint32_t)KeyButton::J;
        case (KeyButton)0x4B:
            return (uint32_t)KeyButton::K;
        case (KeyButton)0x4C:
            return (uint32_t)KeyButton::L;
        case (KeyButton)0x4D:
            return (uint32_t)KeyButton::M;
        case (KeyButton)0x4E:
            return (uint32_t)KeyButton::N;
        case (KeyButton)0x4F:
            return (uint32_t)KeyButton::O;
        case (KeyButton)0x50:
            return (uint32_t)KeyButton::P;
        case (KeyButton)0x51:
            return (uint32_t)KeyButton::Q;
        case (KeyButton)0x52:
            return (uint32_t)KeyButton::R;
        case (KeyButton)0x53:
            return (uint32_t)KeyButton::S;
        case (KeyButton)0x54:
            return (uint32_t)KeyButton::T;
        case (KeyButton)0x55:
            return (uint32_t)KeyButton::U;
        case (KeyButton)0x56:
            return (uint32_t)KeyButton::V;
        case (KeyButton)0x57:
            return (uint32_t)KeyButton::W;
        case (KeyButton)0x58:
            return (uint32_t)KeyButton::X;
        case (KeyButton)0x59:
            return (uint32_t)KeyButton::Y;
        case (KeyButton)0x5A:
            return (uint32_t)KeyButton::Z;

        case KeyButton::LeftBracket:
            return VK_OEM_4;
        case KeyButton::Backslash:
            return VK_OEM_5;
        case KeyButton::RightBracket:
            return VK_OEM_6;
        case KeyButton::GraveAccent:
            return VK_OEM_3;

        // 功能按键
        case KeyButton::Escape:
            return VK_ESCAPE;
        case KeyButton::Enter:
            return VK_RETURN;
        case KeyButton::Tab:
            return VK_TAB;
        case KeyButton::Backspace:
            return VK_BACK;
        case KeyButton::Insert:
            return VK_INSERT;
        case KeyButton::Delete:
            return VK_DELETE;
        case KeyButton::LeftArrow:
            return VK_LEFT;
        case KeyButton::RightArrow:
            return VK_RIGHT;
        case KeyButton::UpArrow:
            return VK_UP;
        case KeyButton::DownArrow:
            return VK_DOWN;
        case KeyButton::PageUp:
            return VK_PRIOR;
        case KeyButton::PageDown:
            return VK_NEXT;
        case KeyButton::Home:
            return VK_HOME;
        case KeyButton::End:
            return VK_END;

        case KeyButton::CapsLock:
            return VK_CAPITAL;
        case KeyButton::NumLock:
            return VK_NUMLOCK;
        case KeyButton::ScrollLock:
            return VK_SCROLL;
        case KeyButton::PrintScreen:
            return VK_SNAPSHOT;
        case KeyButton::Pause:
            return VK_PAUSE;

        // Fn 系列键
        case KeyButton::F1:
            return VK_F1;
        case KeyButton::F2:
            return VK_F2;
        case KeyButton::F3:
            return VK_F3;
        case KeyButton::F4:
            return VK_F4;
        case KeyButton::F5:
            return VK_F5;
        case KeyButton::F6:
            return VK_F6;
        case KeyButton::F7:
            return VK_F7;
        case KeyButton::F8:
            return VK_F8;
        case KeyButton::F9:
            return VK_F9;
        case KeyButton::F10:
            return VK_F10;
        case KeyButton::F11:
            return VK_F11;
        case KeyButton::F12:
            return VK_F12;
        case KeyButton::F13:
            return VK_F13;
        case KeyButton::F14:
            return VK_F14;
        case KeyButton::F15:
            return VK_F15;
        case KeyButton::F16:
            return VK_F16;
        case KeyButton::F17:
            return VK_F17;
        case KeyButton::F18:
            return VK_F18;
        case KeyButton::F19:
            return VK_F19;
        case KeyButton::F20:
            return VK_F20;
        case KeyButton::F21:
            return VK_F21;
        case KeyButton::F22:
            return VK_F22;
        case KeyButton::F23:
            return VK_F23;
        case KeyButton::F24:
            return VK_F24;

            // 小键盘数字键
        case KeyButton::NP_0:
            return VK_NUMPAD0;
        case KeyButton::NP_1:
            return VK_NUMPAD1;
        case KeyButton::NP_2:
            return VK_NUMPAD2;
        case KeyButton::NP_3:
            return VK_NUMPAD3;
        case KeyButton::NP_4:
            return VK_NUMPAD4;
        case KeyButton::NP_5:
            return VK_NUMPAD5;
        case KeyButton::NP_6:
            return VK_NUMPAD6;
        case KeyButton::NP_7:
            return VK_NUMPAD7;
        case KeyButton::NP_8:
            return VK_NUMPAD8;
        case KeyButton::NP_9:
            return VK_NUMPAD9;
        case KeyButton::NP_Decimal:
            return VK_DECIMAL;
        case KeyButton::NP_Div:
            return VK_DIVIDE;
        case KeyButton::NP_Mul:
            return VK_MULTIPLY;
        case KeyButton::NP_Sub:
            return VK_SUBTRACT;
        case KeyButton::NP_Add:
            return VK_ADD;

        case KeyButton::Shift:
            return VK_SHIFT;
        case KeyButton::Control:
            return VK_CONTROL;
        case KeyButton::Alt:
            return VK_MENU;
        case KeyButton::Super:
            return VK_LWIN;

        case KeyButton::LeftShift:
            return VK_LSHIFT;
        case KeyButton::RightShift:
            return VK_RSHIFT;
        case KeyButton::LeftControl:
            return VK_LCONTROL;
        case KeyButton::RightControl:
            return VK_RCONTROL;
        case KeyButton::LeftAlt:
            return VK_LMENU;
        case KeyButton::RightAlt:
            return VK_RMENU;
        case KeyButton::LeftSuper:
            return VK_LWIN;
        case KeyButton::RightSuper:
            return VK_RWIN;
        case KeyButton::Menu:
            return VK_APPS;

        case KeyButton::MediaPlayPause:
            return VK_MEDIA_PLAY_PAUSE;
        case KeyButton::MediaStop:
            return VK_MEDIA_STOP;
        case KeyButton::MediaPrevious:
            return VK_MEDIA_PREV_TRACK;
        case KeyButton::MediaNext:
            return VK_MEDIA_NEXT_TRACK;
        case KeyButton::MediaMute:
            return VK_VOLUME_MUTE;
        case KeyButton::MediaVolumeDown:
            return VK_VOLUME_DOWN;
        case KeyButton::MediaVolumeUp:
            return VK_VOLUME_UP;
        }
        return 0;
    }
    KeyMods PlatWin::getKeyMods()
    {
        KeyMods mods;
        if (GetKeyState(VK_SHIFT) & 0x8000)
            mods.shift = true;
        if (GetKeyState(VK_CONTROL) & 0x8000)
            mods.control = true;
        if (GetKeyState(VK_MENU) & 0x8000)
            mods.alt = true;
        if ((GetKeyState(VK_LWIN) | GetKeyState(VK_RWIN)) & 0x8000)
            mods.super = true;
        if (GetKeyState(VK_CAPITAL) & 1)
            mods.capsLock = true;
        if (GetKeyState(VK_NUMLOCK) & 1)
            mods.numLock = true;
        return mods;
    }
}
