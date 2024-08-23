#include <airkit/Plat/PlatWin/WinWindow.hpp>
#include <windowsx.h>
#include <uxtheme.h>
#include <vssym32.h>
#include "WinWindow.hpp"

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
            auto win = getUIWindow();
            bar->setUIParent(win);
            // 计算窗口标题栏布局
            /*SIZE title_bar_size = {0};
            const int top_and_bottom_borders = 2;
            UINT dpi = GetDpiForWindow(mHWnd);
            auto height = GetSystemMetricsForDpi(SM_CYCAPTION, dpi) + top_and_bottom_borders;
            */
            SIZE title_bar_size = {0};
            const int top_and_bottom_borders = 2;
            HTHEME theme = OpenThemeData(mHWnd, L"WINDOW");
            UINT dpi = GetDpiForWindow(mHWnd);
            GetThemePartSize(theme, NULL, WP_CAPTION, CS_ACTIVE, NULL, TS_TRUE, &title_bar_size);
            CloseThemeData(theme);

            float height = title_bar_size.cy * dpi / 96.0f;
            float half = height / 3*2;
            height += half;

            UIArea size(0, 0, mArea.getWidth(), height);
            UIResizedEvent ev(size);
            mTitleBar->onSized(ev);
        }
    }
    void WinWindow::onSized(UIResizedEvent &event)
    {
        mArea = event.getArea();
        if (mTitleBar.get() != nullptr)
        {
            // 计算窗口标题栏布局
            /*SIZE title_bar_size = {0};
            const int top_and_bottom_borders = 2;
            UINT dpi = GetDpiForWindow(mHWnd);
            auto height = GetSystemMetricsForDpi(SM_CYCAPTION, dpi) + top_and_bottom_borders;
            */
            SIZE title_bar_size = {0};
            const int top_and_bottom_borders = 2;
            HTHEME theme = OpenThemeData(mHWnd, L"WINDOW");
            UINT dpi = GetDpiForWindow(mHWnd);
            GetThemePartSize(theme, NULL, WP_CAPTION, CS_ACTIVE, NULL, TS_TRUE, &title_bar_size);
            CloseThemeData(theme);

            float height = title_bar_size.cy * dpi / 96.0f;
            float half = height / 3*2;
            height += half;

            float y = 0;
           /* // 检查窗口是否最大化
            WINDOWPLACEMENT placement = {0};
            placement.length = sizeof(WINDOWPLACEMENT);
            if (GetWindowPlacement(mHWnd, &placement) &&
                placement.showCmd == SW_SHOWMAXIMIZED)
            {
                int padding = GetSystemMetricsForDpi(SM_CXPADDEDBORDER, dpi);
                y += padding * 2;
            }
            */

            UIArea size(0, y, mArea.getWidth(), height);
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
            // if (hit != HTNOWHERE)
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
        const auto hit = onHitTest(ev);
        if (hit == nullptr)
            return HTCLIENT;

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
            area.setSize(width, height);
            offset += width;
            auto x = mArea.getWidth() - offset;
            area.setPos(x, 2);
            offset += 2;
        }
        if (mUIMaximize.get() != nullptr)
        {
            auto &area = mUIMaximize->getArea();
            area.setSize(width, height);
            offset += width;
            auto x = mArea.getWidth() - offset;
            area.setPos(x, 2);
            offset += 2;
        }
        if (mUIMinimize.get() != nullptr)
        {
            auto &area = mUIMinimize->getArea();
            area.setSize(width, height);
            offset += width;
            auto x = mArea.getWidth() - offset;
            area.setPos(x, 2);
            offset += 2;
        }
        mBtnArea = mArea;
        mBtnArea.setPos(mArea.getWidth() - offset, 0);
        mBtnArea.setSize(offset, mArea.getHeight());
    }
}
