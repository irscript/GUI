#ifndef __WINWINDOW_H__
#define __WINWINDOW_H__

#include <airkit/GUI/UI/IWindow.hpp>
#include <windows.h>
namespace airkit
{
    struct WinWindow : public IWindow
    {
        WinWindow(HWND hd, float x, float y, float w, float h) : mHWnd(hd)
        {
            SetWindowLongPtrA(mHWnd, GWLP_USERDATA, (LONG_PTR)this);
            mArea = {x, y, w, h};
        }
        virtual ~WinWindow() = 0;

        void onEvent(const IEvent &event) override;

        virtual int32_t doModal() override;
        virtual void onCharInput(uint32_t utf32) override;

        virtual LRESULT onHitTest(UIPoint &cursor);
        virtual bool onClose();

        void setHighSurrogate(uint16_t val) { mHighSurrogate = val; }
        uint32_t getHighSurrogate() const { return mHighSurrogate; }

    protected:
        HWND mHWnd;                  // 窗口句柄
        uint32_t mHighSurrogate = 0; // UTF-16编码的高代理字符
    };
}
#endif // __WINWINDOW_H__