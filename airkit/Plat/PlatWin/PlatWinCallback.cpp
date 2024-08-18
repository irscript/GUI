#include <airkit/Plat/PlatWin/PlatWin.hpp>
#include <airkit/Plat/PlatWin/GLWindow.hpp>
#include <airkit/Render/GL/GLRender.hpp>
#define FMT_HEADER_ONLY
#include <airkit/3Part/fmt/core.h>
#include <airkit/3Part/fmt/color.h>

#include <windowsx.h>
#include <uxtheme.h>
#include <vssym32.h>

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
                    UIResizingEvent ev(na);
                    win.onEvent(ev);
                }
                // 判断位置是否有变化
                if (area.getPos().getX() != x || area.getPos().getY() != y)
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
                KeyDownEvent ev(KeyButton(w_param), false);
                win.onEvent(ev);
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
                auto ev = KeyUpEvent(KeyButton(w_param));
                win.onEvent(ev);
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
                /*
                // 判断是否跟踪鼠标悬停
                if (0 == win.getUIFlag(UIFlag::MouseTrack))
                {
                    TRACKMOUSEEVENT tme;
                    tme.cbSize = sizeof(TRACKMOUSEEVENT);
                    tme.dwFlags = TME_HOVER;
                    tme.hwndTrack = handle;
                    tme.dwHoverTime = HOVER_DEFAULT;
                    if (false == TrackMouseEvent(&tme))
                        break;
                }
                */

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
            }
        }
        break;
            /*
             case WM_NCMOUSEHOVER:
             case WM_MOUSEHOVER:
             {
                 auto userdata = GetWindowLongPtr(handle, GWLP_USERDATA);
                 if (userdata != 0)
                 {
                     WinWindow &win = *(WinWindow *)userdata;

                     win.resetUIFlag(UIFlag::MouseTrack);
                     auto x = GET_X_LPARAM(l_param);
                     auto y = GET_Y_LPARAM(l_param);
                     MouseHoverEvent ev(x, y);
                     win.onEvent(ev);
                 }
             }
             break;
             */

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
            }
        }
        break;
        case WM_NCLBUTTONDOWN:
        case WM_LBUTTONDOWN:
            mouse_event_btn<MouseDownEvent>(handle, MouseButton::Left, l_param);
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
            break;
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

        UINT dpi = GetDpiForWindow(handle);
        int frame_y = GetSystemMetricsForDpi(SM_CYFRAME, dpi);
        int padding = GetSystemMetricsForDpi(SM_CXPADDEDBORDER, dpi);

        POINT cursor_point = {0};
        cursor_point.x = GET_X_LPARAM(l_param);
        cursor_point.y = GET_Y_LPARAM(l_param);
        ScreenToClient(handle, &cursor_point);
        ScreenToClient(handle, &cursor_point);
        if (cursor_point.y > 0 && cursor_point.y < frame_y + padding)
        {
            return HTTOP;
        }

        auto userdata = GetWindowLongPtr(handle, GWLP_USERDATA);
        if (userdata == 0)
            return HTCLIENT;

        WinWindow &win = *(WinWindow *)userdata;

        UIPoint cursor(cursor_point.x, cursor_point.y);

        return win.onHitTest(cursor);
    }
}
