#ifndef __GLDRIVER_H__
#define __GLDRIVER_H__

#include <airkit/AIR_PCH.hpp>
#include <airkit/3Part/Glad2/glad/gl.h>

namespace airkit
{
    struct GLDriver
    {
        GLDriver() : mContext() {}

        // 加载上下文
        bool loadContext(GLADloadfunc load) { gladLoadGLContext(&mContext, load) != 0; }

        // 功能函数

        // 设置视口
        void viewport(int x, int y, int width, int height) { mContext.Viewport(x, y, width, height); }
        // 设置清空颜色
        void clearColor(float r, float g, float b, float a) { mContext.ClearColor(r, g, b, a); }
        void clear() { mContext.Clear(GL_COLOR_BUFFER_BIT); }

    private:
        GladGLContext mContext;
    };

}

#endif // __GLDRIVER_H__