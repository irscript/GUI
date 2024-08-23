#ifndef __GLWINDOW_H__
#define __GLWINDOW_H__

#include <airkit/Plat/PlatWin/WinWindow.hpp>

namespace airkit
{

    struct GLWindow : WinWindow
    {
        GLWindow(HWND hd, float x, float y, float w, float h,
                 HDC dc, HGLRC context)
            : WinWindow(hd, x, y, w, h), mWinDC(dc), mGLContext(context) {}

        virtual void prepare() override;
        virtual void present() override;
        virtual void render() override;
        virtual int32_t doModal() override;

        
        const HGLRC getGLContext() const { return mGLContext; }
    protected:
        HDC mWinDC;
        HGLRC mGLContext;
    };
}
#endif // __GLWINDOW_H__