#ifndef __WINWINDOW_H__
#define __WINWINDOW_H__

#include <airkit/GUI/UI/IWindow.hpp>
#include <airkit/GUI/Widgets/TitleBar.hpp>
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

        // 设置标题栏
        void setTitleBar(UIHolder bar);

        // 获取窗口句柄
        HWND getWHD() const { return mHWnd; }

    protected:
        virtual void onSized(UIResizedEvent &event) override;

    protected:
        virtual LRESULT onWinHitTest(UIPoint &cursor);
        virtual bool onClose();

    protected:
        friend struct PlatWin;
        void setHighSurrogate(uint16_t val) { mHighSurrogate = val; }
        uint32_t getHighSurrogate() const { return mHighSurrogate; }

    protected:
        HWND mHWnd;                  // 窗口句柄
        uint32_t mHighSurrogate = 0; // UTF-16编码的高代理字符
        UIHolder mTitleBar;          // 标题栏
    };
    struct WinTitleBar : public TitleBar
    {
        virtual LRESULT onWinHitTest(UIPoint &cursor);

        virtual void onSized(UIResizedEvent &event) override;

        // 计算布局
        void calLayout();
    };
}
#endif // __WINWINDOW_H__