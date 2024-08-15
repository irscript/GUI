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
}
