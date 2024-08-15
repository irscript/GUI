#include <cstdio>
#include <airkit/Plat/PlatWin/PlatWin.hpp>

using namespace airkit;
int main(int argc, char *argv[])
{
    PlatWin plat(RenderAPI::OpenGL);

    auto win = plat.createWindow(1280, 720, "Hello World");

    auto winptr = (IWindow *)win.get();
    winptr->doModal();

    return 0;
}