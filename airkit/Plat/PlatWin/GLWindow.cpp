#include <airkit/Plat/PlatWin/GLWindow.hpp>
#include <airkit/GUI/IPlat.hpp>
#include <airkit/Render/GL/GLRender.hpp>
namespace airkit
{
    void GLWindow::prepare()
    {
        wglMakeCurrent(mWinDC, mGLContext);
        IPlat::getInstance().getRender()->setViewport(0, 0, mArea.getWidth(), mArea.getHeight());
    }

    void GLWindow::present()
    {
        SwapBuffers(mWinDC);
    }
    int32_t GLWindow::doModal()
    {
        prepare();
        auto render = IPlat::getInstance().getRender();

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
            render->clearColor(tick % 3 / 3.0f, tick % 5 / 5.0f, tick % 7 / 7.0f, 1.0f);
            render->clear();
            present();
        }
        return 0;
    }
}
