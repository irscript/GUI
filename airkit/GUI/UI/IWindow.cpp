
#include <airkit/GUI/UI/IWindow.hpp>
#include <airkit/GUI/IPlat.hpp>
#include "IWindow.hpp"

namespace airkit
{

    IWindow::~IWindow() {}
    void IWindow::setShouldClose(bool shouldClose) { shouldClose == true ? mUIFlag.set(UIFlag::WindowClose) : mUIFlag.reset(UIFlag::WindowClose); }

    bool IWindow::shouldClose() const { return mUIFlag.check(UIFlag::WindowClose); }
    bool IWindow::isFullScreen() const { return mUIFlag.check(UIFlag::WindowFullscreen); }
}