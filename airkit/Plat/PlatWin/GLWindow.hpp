#ifndef __GLWINDOW_H__
#define __GLWINDOW_H__

#include <airkit/Plat/PlatWin/WinWindow.hpp>
#include <airkit/Render/GL/GLDriver.hpp>
namespace airkit
{

    struct GLWindow : WinWindow
    {
        GLWindow(HWND h, HDC dc, HGLRC context, GLDriver &gl)
            : WinWindow(h), mWinDC(dc), mGLContext(context), mGL(gl) {}

        virtual void prepare() override;
        virtual void present() override;
        virtual int32_t doModal() override;

    private:
        HDC mWinDC;
        HGLRC mGLContext;
        GLDriver &mGL;
    };
}
#endif // __GLWINDOW_H__