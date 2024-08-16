#include <cstdio>
#include <airkit/Plat/PlatWin/PlatWin.hpp>

using namespace airkit;
int main(int argc, char *argv[])
{
    PlatWin plat;
    plat.init(RenderAPI::OpenGL);

    auto win = plat.createWindow(1280, 720, "Hello World");

    auto render = plat.getRender();
    render->createShader("test", "F:\\gitee\\gui\\demo\\FlatColor.glsl");

    auto winptr = (IWindow *)win.get();
    winptr->doModal();
    plat.shutdown();
    return 0;
}