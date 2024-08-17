#ifndef __PLATWIN_H__
#define __PLATWIN_H__

#include <windows.h>
#include <airkit/GUI/IPlat.hpp>

namespace airkit
{
    // 单例模式类型
    struct PlatWin : public IPlat
    {
        PlatWin();
        ~PlatWin();

        virtual void init(RenderAPI api) override;
        virtual void shutdown() override;

        virtual void error(const std::string &msg) override;

        virtual void warning(const std::string &msg) override;

        // 创建窗口
        UIHolder createWindow(uint32_t width, uint32_t height, const char *title) override;
        // 释放窗口
        void releaseWindow(const IGUIElement *win) override;

    private:
        HWND createWin(uint32_t width, uint32_t height, const char *title);
        static void *GLLoad(const char *name);
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
        static HMODULE mGLDll;

        static const uint32_t DPIBase; // 默认值为 USER_DEFAULT_SCREEN_DPI
    };
}
#endif // __PLATWIN_H__