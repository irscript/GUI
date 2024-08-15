#ifndef __WINWINDOW_H__
#define __WINWINDOW_H__

#include <airkit/GUI/UI/IWindow.hpp>
#include <windows.h>
namespace airkit
{
    struct WinWindow : public IWindow
    {
        WinWindow(HWND h) : mHWnd(h)
        {
            SetWindowLongPtrA(mHWnd, GWLP_USERDATA, (LONG_PTR)this);
        }
        virtual ~WinWindow() = 0;

        void onEvent(const IEvent &event) override;

        virtual int32_t doModal() override;

    protected:
        HWND mHWnd; // 窗口句柄
    };
}
#endif // __WINWINDOW_H__