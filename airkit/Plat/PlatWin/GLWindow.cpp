#include <airkit/Plat/PlatWin/GLWindow.hpp>
#include "GLWindow.hpp"

namespace airkit
{
    void GLWindow::prepare()
    {
        wglMakeCurrent(mWinDC, mGLContext);
    }

    void GLWindow::present()
    {
        SwapBuffers(mWinDC);
    }
    int32_t GLWindow::doModal()
    {
        prepare();

        while (shouldClose() == false)
        {

            {
                MSG message = {0};
                BOOL result = GetMessageW(&message, 0, 0, 0);
                if (result > 0)
                {
                    TranslateMessage(&message);
                    DispatchMessageW(&message);
                }
            }
            auto tick = GetTickCount();
            mGL.clearColor(tick % 3 / 3.0f, tick % 5 / 5.0f, tick % 7 / 7.0f, 1.0f);
            mGL.clear();
            present();
        }
        return 0;
    }
}
