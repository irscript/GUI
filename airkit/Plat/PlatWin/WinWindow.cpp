#include <airkit/Plat/PlatWin/WinWindow.hpp>
#include <windowsx.h>
#include <uxtheme.h>
#include <vssym32.h>

namespace airkit
{

    WinWindow::~WinWindow()
    {
    }

    UIPoint WinWindow::getCursorPos()
    {
        POINT cursor;
        RECT rect;
        if (false == GetCursorPos(&cursor) ||
            false == GetWindowRect(mHWnd, &rect))
            return UIPoint();
        cursor.x -= rect.left;
        cursor.y -= rect.top;
        return UIPoint(cursor.x, cursor.y);
    }

    int32_t WinWindow::doModal()
    {
        MSG msg = {0};
        while (shouldClose() == false)
        {
            PeekMessageA(&msg, mHWnd, 0, 0, PM_REMOVE);
            DispatchMessageA(&msg);
        }

        return 0;
    }

    void WinWindow::setTitleBar(UIHolder bar)
    {
        mTitleBar = bar;
        if (bar.get() != nullptr)
        {
            bar->setUIParent(getUIWindow());
            // 计算窗口标题栏布局
            SIZE title_bar_size = {0};
            const int top_and_bottom_borders = 2;
            UINT dpi = GetDpiForWindow(mHWnd);
            auto height = GetSystemMetricsForDpi(SM_CYCAPTION, dpi) + top_and_bottom_borders;

            UIArea size(0, 0, mArea.getWidth(), height);
            UIResizedEvent ev(size);
            mTitleBar->onSized(ev);
        }
    }

    LRESULT WinWindow::onWinHitTest(UIPoint &cursor)
    {
        auto titleBar = mTitleBar.get();
        if (mTitleBar.get() != nullptr)
        {
            auto hit = ((WinTitleBar *)titleBar)->onWinHitTest(cursor);
            if (hit != HTNOWHERE)
                return hit;
        }
        return HTCLIENT;
    }
    bool WinWindow::onClose()
    {
        return true;
    }
    LRESULT WinTitleBar::onWinHitTest(UIPoint &cursor)
    {
        UIHitEvent ev(cursor);
        auto hit = onHitTest(ev);
        if (hit == nullptr)
            return HTNOWHERE;

        if (hit == mUIIcon.get())
            return HTMENU;

        if (hit == mUIClose.get())
            return HTCLOSE;
        if (hit == mUIMinimize.get())
            return HTMINBUTTON;
        if (hit == mUIMaximize.get())
            return HTMAXBUTTON;

        return HTCAPTION;
    }
    void WinTitleBar::onSized(UIResizedEvent &event)
    {
        mArea = event.getArea();
        calLayout();
    }
    void WinTitleBar::calLayout()
    {
        auto winui = getUIWindow();
        auto win = (WinWindow *)winui.get();
        auto hd = win->getWHD();
        // 按钮宽度
        auto dpi = GetDpiForWindow(hd);
        float width = 47.0 * (dpi / 96.0);
        auto height = mArea.getHeight() - 4.0;

        // 计算图标的位置
        if (mUIIcon.get() != nullptr)
        {
            auto &area = mUIIcon->getArea();
            area.setArea(2, 2, width, height);
        }
        float offset = 2;
        // 计算关闭按钮的位置
        if (mUIClose.get() != nullptr)
        {
            auto &area = mUIClose->getArea();
            offset += width;
            area.setArea(mArea.getWidth() - offset, 2, width, height);
        }
        if (mUIMaximize.get() != nullptr)
        {
            auto &area = mUIMaximize->getArea();
            offset += width;
            area.setArea(mArea.getWidth() - offset, 2, width, height);
        }
        if (mUIMinimize.get() != nullptr)
        {
            auto &area = mUIMinimize->getArea();
            offset += width;
            area.setArea(mArea.getWidth() - offset, 2, width, height);
        }
        mBtnArea = mArea;
        mBtnArea.setX(mArea.getWidth() - offset);
    }
}
