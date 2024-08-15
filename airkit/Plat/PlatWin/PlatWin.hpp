#ifndef __PLATWIN_H__
#define __PLATWIN_H__

#include <windows.h>
#include <airkit/GUI/IPlat.hpp>
#include <airkit/Render/GL/GLDriver.hpp>

namespace airkit
{
    // 单例模式类型
    struct PlatWin : public IPlat
    {
        PlatWin(RenderAPI api);
        ~PlatWin();

        // 创建窗口
        UIPtr createWindow(uint32_t width, uint32_t height, const char *title);

    private:
        HWND createWin(uint32_t width, uint32_t height, const char *title);
        static GLADapiproc GLLoad(const char *name);
        IWindow *createGLWin(uint32_t width, uint32_t height, const char *title);

    private:
        void registerClass();
        static LRESULT wincallback(HWND handle, UINT message, WPARAM w_param, LPARAM l_param);
        static LRESULT onWM_NCCALCSIZE(HWND handle, UINT message, WPARAM w_param, LPARAM l_param);
        static LRESULT onWM_NCHITTEST(HWND handle, UINT message, WPARAM w_param, LPARAM l_param);
    private:
        PlatWin(const PlatWin &) = delete;
        PlatWin &operator=(const PlatWin &) = delete;

        const char *mWinCls; // 窗口类名称
        bool mGLInit;        // 是否初始化opengl驱动
        GLDriver mGlDriver;
        static HMODULE mGLDll;
    };
}
#endif // __PLATWIN_H__