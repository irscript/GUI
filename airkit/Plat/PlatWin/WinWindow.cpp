#include <airkit/Plat/PlatWin/WinWindow.hpp>
#include "WinWindow.hpp"

namespace airkit
{

    WinWindow::~WinWindow()
    {
    }
    void airkit::WinWindow::onEvent(const IEvent &event)
    {
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
    void WinWindow::onCharInput(uint32_t utf32)
    {
        printf("char:%X\n", utf32);
    }
    LRESULT WinWindow::onHitTest(UIPoint &cursor)
    {
        return HTCLIENT;
    }
    bool WinWindow::onClose()
    {
        return true;
    }
}
