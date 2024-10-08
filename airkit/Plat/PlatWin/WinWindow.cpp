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

            SIZE title_bar_size = {0};
            const int top_and_bottom_borders = 2;
            HTHEME theme = OpenThemeData(mHWnd, L"WINDOW");
            UINT dpi = GetDpiForWindow(mHWnd);
            GetThemePartSize(theme, NULL, WP_CAPTION, CS_ACTIVE, NULL, TS_TRUE, &title_bar_size);
            CloseThemeData(theme);

            float height = title_bar_size.cy * dpi / 96.0f;
            float half = height / 3 * 2;
            height += half;

            UIArea size(0, 0, mArea.getWidth(), height);
            UIResizedEvent ev(size);
            mTitleBar->onSized(ev);

            if (mContent.get() != nullptr)
            {
                UIArea size(0, height, mArea.getWidth(), mArea.getHeight() - height);
                UIResizedEvent ev(size);
                mContent->onSized(ev);
            }
        }
    }
    void WinWindow::setContent(UIHolder content)
    {
        mContent = content;
        if (content.get() != nullptr)
        {
            float height = 0;
            if (mTitleBar.get() != nullptr)
                height = mTitleBar->getArea().getHeight();
            UIArea size(0, height, mArea.getWidth(), mArea.getHeight() - height);
            UIResizedEvent ev(size);
            mContent->onSized(ev);
        }
    }
    void WinWindow::onSized(UIResizedEvent &event)
    {
        float y = 0;
        mArea = event.getArea();
        if (mTitleBar.get() != nullptr)
        {
            // 计算窗口标题栏布局

            SIZE title_bar_size = {0};
            const int top_and_bottom_borders = 2;
            HTHEME theme = OpenThemeData(mHWnd, L"WINDOW");
            UINT dpi = GetDpiForWindow(mHWnd);
            GetThemePartSize(theme, NULL, WP_CAPTION, CS_ACTIVE, NULL, TS_TRUE, &title_bar_size);
            CloseThemeData(theme);

            float height = title_bar_size.cy * dpi / 96.0f;
            float half = height / 3 * 2;
            height += half;

            y = height;

            UIArea size(0, 0, mArea.getWidth(), height);
            UIResizedEvent ev(size);
            mTitleBar->onSized(ev);
        }
        if (mContent.get() != nullptr)
        {
            UIArea size(0, y, mArea.getWidth(), mArea.getHeight() - y);
            UIResizedEvent ev(size);
            mContent->onSized(ev);
        }

        render();
    }
    IGUIElement *WinWindow::onHitTest(const UIHitEvent &event)
    {
        if (mTitleBar.get() != nullptr)
        {
            auto hit = mTitleBar->onHitTest(event);
            if (hit != nullptr)
                return hit;
        }
        return nullptr;
    }
    void WinWindow::onMouseMove(MouseMoveEvent &event)
    {
        auto pos = event.getPos();
        auto hit = onHitTest(UIHitEvent(pos));
        mUIVibe.mHover = hit;
        if (hit != nullptr)
        {
            hit->onEvent(event);
        }
    }
    void WinWindow::onMouseClick(MouseDownEvent &event)
    {
        auto pos = event.getPos();
        auto hit = onHitTest(UIHitEvent(pos));
        mUIVibe.mFocus = hit;
        mUIVibe.mHover = hit;
        if (hit != nullptr)
        {
            hit->onEvent(event);
        }
    }
    void WinWindow::onMouseUp(MouseUpEvent &event)
    {
        auto hit = mUIVibe.mHover;
        // auto pos = event.getPos();
        // auto hit = onHitTest(UIHitEvent(pos));
        if (hit != nullptr)
        {
            hit->onEvent(event);
        }
    }
    void WinWindow::maximize()
    {
        ShowWindow(mHWnd, SW_MAXIMIZE);
    }
    void WinWindow::restore()
    {
        ShowWindow(mHWnd, SW_RESTORE);
    }
    void WinWindow::minimize()
    {
        ShowWindow(mHWnd, SW_MINIMIZE);
    }
    void WinWindow::close()
    {
        PostMessageA(mHWnd, WM_CLOSE, 0, 0);
    }
    LRESULT WinWindow::onWinHitTest(UIPoint &cursor)
    {
        auto titleBar = mTitleBar.get();
        if (mTitleBar.get() != nullptr)
        {
            auto hit = ((WinTitleBar *)titleBar)->onWinHitTest(cursor);
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

        // HTTOP
        if (cursor.mY <= 3)
            return HTTOP;

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
