#ifndef __WINWINDOW_H__
#define __WINWINDOW_H__

#include <airkit/GUI/UI/IWindow.hpp>
#include <windows.h>
namespace airkit
{
    struct WinWindow : public IWindow
    {
        WinWindow(HWND hd, float x, float y, float w, float h) : IWindow(), mHWnd(hd)
        {
            SetWindowLongPtrA(mHWnd, GWLP_USERDATA, (LONG_PTR)this);
            mArea = {x, y, w, h};
        }
        virtual ~WinWindow() = 0;

        virtual UIPoint getCursorPos() override; // 获取鼠标位置，窗口坐标

        

        virtual int32_t doModal() override;

        virtual LRESULT onHitTest(UIPoint &cursor);
        virtual bool onClose();

    protected:
        friend struct PlatWin;
        void setHighSurrogate(uint16_t val) { mHighSurrogate = val; }
        uint32_t getHighSurrogate() const { return mHighSurrogate; }

    protected:
        HWND mHWnd;                  // 窗口句柄
        uint32_t mHighSurrogate = 0; // UTF-16编码的高代理字符
    };
}
#endif // __WINWINDOW_H__