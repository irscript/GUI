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
            {"aPos", ShaderDataType::Float2},
            {"aClr", ShaderDataType::Float4},
        };
        // auto ubosize = shader->bindUniformBuffer("uPushConstant", 0);
        // mUBO = render->createUniformBuffer(ubosize, 0);
        //  创建管线
        mPipeline = render->createPipeline("UI", layout, shader);

        // 创建顶点数组
        mVAO = render->createVertexArray();
        // 创建顶点缓冲
        float vertices[] = {
            200.0f,
            400.0f, // top right
            1.0f,
            0.0f,
            0.0f,
            1.0f,

            400,
            400, // bottom right
            0.0f,
            1.0f,
            0.0f,
            1.0f,

            400,
            200,
            // bottom left
            0.0f,
            0.0f,
            1.0f,
            1.0f,

            200,
            200,
            // top left
            1.0f,
            1.0f,
            1.0f,
            1.0f,
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
                BOOL bRet = PeekMessageW(&message, mHWnd, 0, 0, PM_REMOVE);
                if (bRet)
                {
                    TranslateMessage(&message);
                    DispatchMessageW(&message);
                }
            }

            prepare();

            render->clearColor(1.0f, 1.0f, 1.0f, 1.0f);
            render->clear();

            mPipeline->bind(); // 先生成数据
            auto wsize = mArea.getSize();
            printf("wsize: %f, %f\n", wsize.getWidth(), wsize.getHeight());
            float ubo[4];
            ubo[0] = 2.0f / wsize.getWidth();
            ubo[1] = 2.0f / wsize.getHeight();
            ubo[2] = -wsize.getWidth() / 2.0f * ubo[0];
            ubo[3] = -wsize.getHeight() / 2.0f * ubo[1];
            printf("ubo: %f, %f, %f, %f\n", ubo[0], ubo[1], ubo[2], ubo[3]);
            // mUBO->setData(ubo, 16);

            auto shader = mPipeline->getShader();
            shader->setFloat2("uScale", ubo);
            shader->setFloat2("uTranslate", ubo + 2);

            mVAO->bind();

            /*auto tick = GetTickCount();
            if ((tick % 10) < 5)
                render->drawIndexs(3, 3, false);
            else
                render->drawIndexs(0, 3, false);
            */
            render->drawIndexs(0, 6, false);

            present();
        }
    }
    virtual LRESULT onHitTest(UIPoint &cursor) override
    {
        return HTCLIENT;
    }

private:
    PipelineHolder mPipeline;
    VAOHolder mVAO;
    VBOHolder mVBO;
    IBOHolder mIBO;
    UBOHolder mUBO;
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

    Window *winptr = (Window *)win.get();
    winptr->init("UIProgram.glsl");

    while (winptr->shouldClose() == false)
    {
        winptr->render();
    }

    plat.shutdown();

    return 0;
}