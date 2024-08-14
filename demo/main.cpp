#include <cstdio>
#include <airkit/Core/Bitop.hpp>
#include <airkit/3Part/Glad2/glad/gl.h>

#include <windows.h>

HMODULE glhd;

static GLADloadfunc getProcAddressWGL(const char* procname)
{
    const GLADloadfunc proc = (GLADloadfunc) wglGetProcAddress(procname);
    if (proc)
        return proc;

    return (GLADloadfunc) GetProcAddress(glhd, procname);
}
int main(int argc, char *argv[])
{
    GladGLContext gl = {};

    glhd = LoadLibraryA("opengl32.dll");

    auto ret = gladLoadGLContext(&gl, (GLADloadfunc)getProcAddressWGL);

    return 0;
}