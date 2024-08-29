#include <cstdio>
#include <airkit/Plat/PlatWin/PlatWin.hpp>
#include <airkit/Plat/PlatWin/GLWindow.hpp>
#include <airkit/GUI/UI/UIColor.hpp>
#include <airkit/GUI/UI/UIHelper.hpp>
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
            {"aUV", ShaderDataType::Float2},
            {"aClr", ShaderDataType::Int},
        }; //  创建管线
        mPipeline = render->createPipeline("UI", layout, shader);
        mVBO = render->createVertexBuffer(mPipeline->getVertexLayout(), 48);
        // 创建索引缓冲
        mIBO = render->createIndexBuffer(12);

        mVAO = render->createVertexArray();
        mVAO->addVertexBuffer(mVBO);
        mVAO->setIndexBuffer(mIBO);
        // auto ubosize = shader->bindUniformBuffer("uPushConstant", 0);
        // mUBO = render->createUniformBuffer(ubosize, 0);

        mTexture = render->createTexture2D("A_letter.png"); //"bug.png");
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
                /*
                {
                    MSG message = {0};
                    BOOL result = GetMessageW(&message, 0, 0, 0);
                    if (result > 0)
                    {
                        TranslateMessage(&message);
                        DispatchMessageW(&message);
                    }
                }*/
            }

            prepare();

            render->clearColor(0.0f, 1.0f, 1.0f, 1.0f);
            render->clear();

            mPipeline->bind(); // 先生成数据
            auto wsize = mArea.getSize();

            float ubo[4];
            ubo[0] = 2.0f / wsize.getWidth();
            ubo[1] = 2.0f / wsize.getHeight();
            ubo[2] = -1.0f; //-wsize.getWidth() / 2.0f * ubo[0];
            ubo[3] = -1.0f; //-wsize.getHeight() / 2.0f * ubo[1];

            // mUBO->setData(ubo, 16);

            auto shader = mPipeline->getShader();
            shader->setFloat2("uScale", ubo);
            shader->setFloat2("uTranslate", ubo + 2);

            // 创建顶点数组
            // mVAO = render->createVertexArray();
            // 创建顶点缓冲
            auto w = wsize.getWidth();
            auto h = wsize.getHeight();

            UIDrawList mDrawList;
            UIHelper ui(mDrawList);

            auto &cmd = mDrawList.mDrawCommands.emplace_back();
            cmd.mStartIndex = 0;
            cmd.mIndexCount = 6;

            // 绘制线条
            ui.drawLine(UIPoint(0, 10), UIPoint(w, 10), RGBA(255, 0, 0, 255), 5);
            ui.drawLine(UIPoint(0, 20), RGBA(255, 0, 0, 255), UIPoint(w, 20), RGBA(0, 255, 0, 255), 5);

            // 绘制矩形
            ui.drawRect(UIArea(10, 30, 10, 10), RGBA(255, 0, 0, 255), 5);
            ui.drawRect(UIArea(30, 30, 10, 10), RGBA(255, 0, 0, 255), RGBA(0, 255, 0, 255), 5);
            ui.drawRect(UIArea(50, 30, 10, 10), RGBA(255, 0, 0, 255), RGBA(0, 255, 0, 255), RGBA(0, 0, 255, 255), RGBA(0, 0, 0, 255), 5);
            ui.drawRect2(UIArea(70, 30, 10, 10), RGBA(255, 0, 0, 255), RGBA(0, 255, 0, 255), 5);

            ui.drawRect(UIPoint(10, 50), UIPoint(20, 60), RGBA(255, 0, 0, 255), 4);
            ui.drawRect(UIPoint(30, 50), UIPoint(40, 60), RGBA(255, 0, 0, 255), RGBA(0, 255, 0, 255), 4);
            ui.drawRect(UIPoint(50, 50), UIPoint(60, 60), RGBA(255, 0, 0, 255), RGBA(0, 255, 0, 255), RGBA(0, 0, 255, 255), RGBA(0, 0, 0, 255), 4);
            ui.drawRect2(UIPoint(70, 50), UIPoint(80, 60), RGBA(255, 0, 0, 255), RGBA(0, 255, 0, 255), 4);
            // 填充矩形
            ui.fillRect(UIArea(10, 70, 10, 10), RGBA(255, 0, 0, 255));
            ui.fillRect(UIArea(30, 70, 10, 10), RGBA(255, 0, 0, 255), RGBA(0, 255, 0, 255));
            ui.fillRect(UIArea(50, 70, 10, 10), RGBA(255, 0, 0, 255), RGBA(0, 255, 0, 255), RGBA(0, 0, 255, 255), RGBA(0, 0, 0, 255));
            ui.fillRect2(UIArea(70, 70, 10, 10), RGBA(255, 0, 0, 255), RGBA(0, 255, 0, 255));

            ui.fillRect(UIPoint(10, 90), UIPoint(20, 100), RGBA(255, 0, 0, 255));
            ui.fillRect(UIPoint(30, 90), UIPoint(40, 100), RGBA(255, 0, 0, 255), RGBA(0, 255, 0, 255));
            ui.fillRect(UIPoint(50, 90), UIPoint(60, 100), RGBA(255, 0, 0, 255), RGBA(0, 255, 0, 255), RGBA(0, 0, 255, 255), RGBA(0, 0, 0, 255));
            ui.fillRect2(UIPoint(70, 90), UIPoint(80, 100), RGBA(255, 0, 0, 255), RGBA(0, 255, 0, 255));

            // 绘制圆
            ui.drawCircle(UIPoint(100, 200), 60.0f,
                          RGBA(255, 0, 0, 255), RGBA(0, 255, 0, 255), 20, 3);
            ui.fillCircle(UIPoint(100, 400), 60.0f, RGBA(0, 255, 0, 255), 16);
            ui.fillCircle(UIPoint(300, 400), 100.0f, RGBA(255, 0, 0, 255), RGBA(0, 255, 0, 255), 16);

            // 绘制点
            ui.drawPoint(UIPoint(100, 100), RGBA(0, 0, 255, 255), 12);

            // 绘制三角形
            ui.drawTriangle(UIPoint(400, 100), RGBA(255, 0, 0),
                            UIPoint(500, 130), RGBA(0, 255, 0),
                            UIPoint(350, 210), RGBA(0, 0, 255),
                            12);
            ui.fillTriangle(UIPoint(200, 100), UIPoint(300, 100), UIPoint(200, 200), RGBA(0, 0, 255));

            // 绘制弧线
            ui.fillSector(UIPoint(500, 400), 80.0f, 0.0f, 90.0f,
                          RGBA(255, 0, 0, 255), RGBA(0, 255, 0), 2);
            ui.drawArc(UIPoint(510, 410), 80.0f, 0.0f, 90.0f,
                       RGBA(255, 0, 0, 255), RGBA(0, 255, 0), 16, 2);

            // 绘制纹理
            RGBA clrarr[4] = {
                RGBA(255, 0, 0, 255),
                RGBA(0, 255, 0, 255),
                RGBA(0, 0, 255, 255),
                RGBA(255, 255, 0, 255)};
            ui.texRect(UIPoint(w / 3.0f, h / 3.0f), UIPoint(0, 1),
                       UIPoint(w / 3.0f * 2, h / 3.0f * 2), UIPoint(1, 0), clrarr);

            mVBO->setData(mDrawList.mVertices.data(), mDrawList.mVertices.size() * sizeof(UIVertex));
            auto icount = mDrawList.mIndices.size();
            auto isize = icount * sizeof(UIIndex);
            mIBO->setData(mDrawList.mIndices.data(), isize, icount);
            mVAO->bind();

            shader->setInt("drawflag", 1);
            render->drawIndexs(0, icount - 6, false);
            shader->setInt("drawflag", 2);
            mTexture->bind();
            // shader->setInt("drawflag", 2);
            render->drawIndexs(icount - 6, 6, false);
            mVAO->unbind();
            // Sleep(100);
            present();
        }
    }

private:
    PipelineHolder mPipeline;
    VAOHolder mVAO;
    VBOHolder mVBO;
    IBOHolder mIBO;
    UBOHolder mUBO;
    TextureHolder mTexture;
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
    winptr->init("UIDefault.glsl");

    while (winptr->shouldClose() == false)
    {
        winptr->render();
    }

    plat.shutdown();

    return 0;
}