#include <cstdio>
#include <airkit/Plat/PlatWin/PlatWin.hpp>
#include <airkit/Plat/PlatWin/GLWindow.hpp>
using namespace airkit;

struct Window : public GLWindow
{
    Window(HWND hd, float x, float y, float w, float h,
           HDC dc, HGLRC context)
        : GLWindow(hd, x, y, w, h, dc, context) {}

    void init(const char *name)
    {
        auto render = IPlat::getInstance().getRender();
        wglMakeCurrent(mWinDC, mGLContext);
        // 创建着色器
        auto shader = render->createShader(name, name);
        VertexLayout layout = {
            {"aPos", ShaderDataType::Float3},
        };
        // 创建管线
        mPipeline = render->createPipeline("default", layout, shader);
        // 创建顶点数组
        mVAO = render->createVertexArray();
        // 创建顶点缓冲
        float vertices[] = {
            0.5f, 0.5f, 0.0f,   // top right
            0.5f, -0.5f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f, // bottom left
            -0.5f, 0.5f, 0.0f   // top left
        };
        uint16_t indices[] = {
            // note that we start from 0!
            0, 1, 3, // first Triangle
            1, 2, 3  // second Triangle
        };
        mVBO = render->createVertexBuffer(mPipeline->getVertexLayout(), vertices, sizeof(vertices));
        // 创建索引缓冲
        mIBO = render->createIndexBuffer(indices, sizeof(indices), 6);
        mVAO->addVertexBuffer(mVBO);
        mVAO->setIndexBuffer(mIBO);
    }
    virtual void render() override
    {

        if (shouldClose() == false)
        {
            auto render = IPlat::getInstance().getRender();
            {
                MSG message = {0};
                /* BOOL result = GetMessageW(&message, mHWnd, 0, 0);
                 if (result > 0)
                 {
                     TranslateMessage(&message);
                     DispatchMessageW(&message);
                 }*/
                BOOL bRet = PeekMessageW(&message, mHWnd, 0, 0, PM_REMOVE);
                if (bRet)
                    DispatchMessageW(&message);
            }
            prepare();

            render->clearColor(1.0f, 1.0f, 1.0f, 1.0f);
            render->clear();

            mPipeline->bind();
            mVAO->bind();
            auto tick = GetTickCount();
            if ((tick % 10) < 5)
                render->drawIndexs(3, 3, false);
            else
                render->drawIndexs(0, 3, false);

            present();
        }
    }
    

private:
    PipelineHolder mPipeline;
    VAOHolder mVAO;
    VBOHolder mVBO;
    IBOHolder mIBO;
};

struct Plat : public PlatWin
{
protected:
    virtual IWindow *createGLWin(HWND hd, float x, float y,
                                 float w, float h,
                                 HDC dc, HGLRC context) override
    {
        return new Window(hd, x, y, w, h, dc, context);
    }
};

int main(int argc, char *argv[])
{
    Plat plat;
    plat.init(RenderAPI::OpenGL);

    auto win = plat.createWindow(800, 600, "win", {});
    auto win2 = plat.createWindow(800, 600, "win2", win);

    Window *winptr = (Window *)win.get();
    Window *winptr2 = (Window *)win2.get();
    winptr->init("FlatColor.glsl");
    winptr2->init("FlatColor2.glsl");

    while (true)
    {
        int i = 2;
        if (winptr->shouldClose() == false)
            winptr->render();
        else
            i--;
        if (winptr2->shouldClose() == false)
            winptr2->render();
        else
            i--;
        if (i == 0)
            break;
    }

    plat.shutdown();

    return 0;
}