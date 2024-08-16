#ifndef __WINWINDOW_H__
#define __WINWINDOW_H__

#include <airkit/GUI/UI/IWindow.hpp>
#include <windows.h>
namespace airkit
{
    struct WinWindow : public IWindow
    {
        WinWindow(HWND hd,float x,float y,float w,float h) : mHWnd(hd)
        {
            SetWindowLongPtrA(mHWnd, GWLP_USERDATA, (LONG_PTR)this);
            mArea = {x,y,w,h};
        }
        virtual ~WinWindow() = 0;

        void onEvent(const IEvent &event) override;

        virtual int32_t doModal() override;

        virtual LRESULT onHitTest(UIPoint& cursor);

    protected:
        HWND mHWnd; // 窗口句柄
    };
}
#endif // __WINWINDOW_H__