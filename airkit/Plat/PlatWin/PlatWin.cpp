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
    HMODULE PlatWin::mGLDll = 0;
    const uint32_t PlatWin::DPIBase = USER_DEFAULT_SCREEN_DPI;

    PlatWin::PlatWin()
        : IPlat(),
          mGLInit(false)
    {
        mWinCls = "airkit-window";
        registerClass();
        mInstance = this;
    }
    PlatWin::~PlatWin()
    {
        // 释放DLL
        if (!mGLDll)
            FreeLibrary(mGLDll);
        // 注销类
        UnregisterClassA(mWinCls, nullptr);
    }

    UIHolder PlatWin::createWindow(uint32_t width, uint32_t height, const char *title)
    { // TODO:多线程安全
        IWindow *win;
        switch (mRenderAPI)
        {
        case RenderAPI::OpenGL:
            win = createGLWin(width, height, title);
            break;

        default:
            error("unknown render API");
            break;
        }
        // 添加到窗口管理器
        return mWinHub.addWindow(win);
    }

    void PlatWin::releaseWindow(const IGUIElement *win)
    {
        // TODO:多线程安全
        mWinHub.removeWindow(win);
    }

    void PlatWin::init(RenderAPI api)
    {
        mRenderAPI = api;
    }
    void PlatWin::shutdown()
    {
        if (nullptr != mRender.get())
            mRender->shutdown();
        // 释放DLL
        if (!mGLDll)
            FreeLibrary(mGLDll);
        mGLInit = false;
        // 注销类
        UnregisterClassA(mWinCls, nullptr);
    }
    void PlatWin::error(const std::string &msg)
    {
        fmt::print(fg(fmt::color::red) | fmt::emphasis::bold, "Error: {}\n", msg);
        exit(-1);
    }
    void PlatWin::warning(const std::string &msg)
    {
        fmt::print(fg(fmt::color::yellow) | fmt::emphasis::bold, "Warning: {}\n", msg);
    }
    HWND PlatWin::createWin(uint32_t width, uint32_t height, const char *title)
    {
        int window_style = WS_THICKFRAME    // required for a standard resizeable window
                           | WS_SYSMENU     // Explicitly ask for the titlebar to support snapping via Win + ← / Win + →
                           | WS_MAXIMIZEBOX // Add maximize button to support maximizing via mouse dragging
                                            // to the top of the screen
                           | WS_MINIMIZEBOX // Add minimize button to support minimizing by clicking on the taskbar icon
                           | WS_VISIBLE;    // Make window visible after it is created (not important)
        auto hWnd = CreateWindowExA(
            WS_EX_APPWINDOW,
            mWinCls,
            title,
            // The
            window_style,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            width, height,
            0,
            0,
            0,
            0);
        auto err = GetLastError();
        if (hWnd == 0)
        {
            error(fmt::format("CreateWindowExA failed: {}\n", err));
        }
        return hWnd;
    }

    void *PlatWin::GLLoad(const char *name)
    {
        auto ret = wglGetProcAddress(name);
        if (ret != nullptr)
            return (void *)ret;
        return (void *)GetProcAddress(PlatWin::mGLDll, name);
    }

    // 创建GL窗口
    IWindow *PlatWin::createGLWin(uint32_t width, uint32_t height, const char *title)
    {
        // 创建窗口
        auto whd = createWin(width, height, title);
        // 创建GL上下文
        HGLRC glrc = nullptr;
        HDC wdc = 0;
        {
            PIXELFORMATDESCRIPTOR pfd =
                {
                    sizeof(PIXELFORMATDESCRIPTOR),
                    1,
                    PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, // Flags
                    PFD_TYPE_RGBA,                                              // The kind of framebuffer. RGBA or palette.
                    32,                                                         // Colordepth of the framebuffer.
                    0, 0, 0, 0, 0, 0,
                    0,
                    0,
                    0,
                    0, 0, 0, 0,
                    24, // Number of bits for the depthbuffer
                    8,  // Number of bits for the stencilbuffer
                    0,  // Number of Aux buffers in the framebuffer.
                    PFD_MAIN_PLANE,
                    0,
                    0, 0, 0};

            wdc = GetDC(whd);

            int letWindowsChooseThisPixelFormat;
            letWindowsChooseThisPixelFormat = ChoosePixelFormat(wdc, &pfd);
            SetPixelFormat(wdc, letWindowsChooseThisPixelFormat, &pfd);

            glrc = wglCreateContext(wdc);
            wglMakeCurrent(wdc, glrc);

            if (mGLInit == false)
            {
                mGLDll = LoadLibraryA("opengl32.dll");
                if (mGLDll == 0)
                    error("LoadLibrary failed: opengl32.dll");

                mRender = RenderHolder(new GLRender());
                if (false == mRender->init((void *)GLLoad))
                    error("gladLoadGLLoader failed");

                mGLInit = true;
            }
        }

        RECT size_rect;
        GetWindowRect(whd, &size_rect);

        // Inform the application of the frame change to force redrawing with the new
        // client area that is extended into the title bar
        width = size_rect.right - size_rect.left;
        height = size_rect.bottom - size_rect.top;

        // 创建GL窗口
        return new GLWindow(whd,
                            size_rect.left, size_rect.top,
                            width, height,
                            wdc, glrc);
    }

    void airkit::PlatWin::registerClass()
    {
        WNDCLASSEXA window_class = {0};
        {
            window_class.cbSize = sizeof(window_class);
            window_class.lpszClassName = mWinCls;
            // Set the procedure that will receive window messages (events)
            window_class.lpfnWndProc = wincallback;
            // Ask to send WM_PAINT when resizing horizontally and vertically
            window_class.style = CS_HREDRAW | CS_VREDRAW;
        }
        auto ret = RegisterClassExA(&window_class);
        if (ret == 0)
            error(fmt::format("RegisterClassEx failed: {}\n", GetLastError()));
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
                    win.onCharInput(codepoint);
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
                win.onCharInput(w_param);
            }
            return 0;
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
