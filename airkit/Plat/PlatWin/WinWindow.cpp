#include <airkit/Plat/PlatWin/WinWindow.hpp>
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

    LRESULT WinWindow::onHitTest(UIPoint &cursor)
    {
         return  HTCLIENT;
       // return HTCAPTION;
    }
    bool WinWindow::onClose()
    {
        return true;
    }
}
