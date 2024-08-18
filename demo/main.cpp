#include <cstdio>
#include <airkit/Plat/PlatWin/PlatWin.hpp>

using namespace airkit;
int main(int argc, char *argv[])
{
    PlatWin plat;
    plat.init(RenderAPI::OpenGL);

    auto win = plat.createWindow(800, 600, "Hello World");

    auto render = plat.getRender();
     //render->createShader("test", "E:\\Project\\gui\\demo\\FlatColor.glsl");

    auto winptr = (IWindow *)win.get();
    //win->getUILimit().setH(UISize(0, 800));
    //win->getUILimit().setV(UISize(0, 600));
    winptr->doModal();
    plat.shutdown();
    return 0;
}