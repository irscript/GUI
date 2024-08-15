#include <airkit/Plat/PlatWin/PlatWin.hpp>
#include <airkit/Plat/PlatWin/GLWindow.hpp>
#include "PlatWin.hpp"

namespace airkit
{
    HMODULE PlatWin::mGLDll = 0;

    PlatWin::PlatWin(RenderAPI api)
        : IPlat(api),
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

    UIPtr PlatWin::createWindow(uint32_t width, uint32_t height, const char *title)
    {
        IWindow *win;
        switch (mRenderAPI)
        {
        case RenderAPI::OpenGL:
            win = createGLWin(width, height, title);
            break;

        default:
            break;
        }
        // 添加到窗口管理器
        return mWinHub.addWindow(win);
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
        return hWnd;
    }

    GLADapiproc PlatWin::GLLoad(const char *name)
    {
        auto ret = wglGetProcAddress(name);
        if (ret != nullptr)
            return (GLADapiproc)ret;
        return (GLADapiproc)GetProcAddress(PlatWin::mGLDll, name);
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
                mGlDriver.loadContext(GLLoad);
                mGLInit = true;
            }
        }

        // 创建GL窗口
        return new GLWindow(whd, wdc, glrc, mGlDriver);
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
            SetWindowPos(
                handle, NULL,
                size_rect.left, size_rect.top,
                size_rect.right - size_rect.left, size_rect.bottom - size_rect.top,
                SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
            break;
        }
        case WM_SETCURSOR:
        {
            // Show an arrow instead of the busy cursor
            SetCursor(LoadCursor(NULL, IDC_ARROW));
            break;
        }
        case WM_DESTROY:
        {
            auto userdata = GetWindowLongPtr(handle, GWLP_USERDATA);

            WinWindow &win = *(WinWindow *)userdata;
            win.setShouldClose(true);

            return 0;
        }
        default:
            break;
        }
        return DefWindowProcA(handle, message, w_param, l_param);
    }
}

// Handling this event allows us to extend client (paintable) area into the title bar region
// The information is partially coming from:
// https://docs.microsoft.com/en-us/windows/win32/dwm/customframe#extending-the-client-frame
// Most important paragraph is:
//   To remove the standard window frame, you must handle the WM_NCCALCSIZE message,
//   specifically when its wParam value is TRUE and the return value is 0.
//   By doing so, your application uses the entire window region as the client area,
//   removing the standard frame.
LRESULT airkit::PlatWin::onWM_NCCALCSIZE(HWND handle, UINT message, WPARAM w_param, LPARAM l_param)
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

LRESULT airkit::PlatWin::onWM_NCHITTEST(HWND handle, UINT message, WPARAM w_param, LPARAM l_param)
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
    return HTCLIENT;
}
