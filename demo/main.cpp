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

        mTexture = render->createTexture2D("bug.png");
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

            render->clearColor(1.0f, 1.0f, 1.0f, 1.0f);
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
            /*
                        RGBA clr(255, 192, 203, 255);
                        float color = *(float *)&clr;
                        float vertices[] = {
                            w / 4.0f * 3,
                            h / 4.0f, // top right
                            1.0f,
                            1.0f,
                            RGBA::fromRGBA(255, 0, 0, 255),

                            w / 4.0f * 3,
                            h / 4.0f * 3, // bottom right
                            1.0f,
                            0.0f,
                            RGBA::fromRGBA(0, 255, 0, 255),

                            w / 4.0f,
                            h / 4.0f * 3, // bottom left
                            0.0f,
                            0.0f,
                            RGBA::fromRGBA(0, 0, 255, 255),

                            w / 4.0f,
                            h / 4.0f, // top left
                            0.0f,
                            1.0f,
                            RGBA::fromRGBA(255, 255, 255, 255),
                        };

                        uint16_t indices[] = {
                            // note that we start from 0!
                            0, 1, 3, // first Triangle
                            1, 2, 3  // second Triangle
                        };

                        mVBO->setData(vertices, sizeof(vertices));
                        mIBO->setData(indices, sizeof(indices), 6);

                        mVAO->bind();
                        mTexture->bind();
                        shader->setInt("drawfalg", 1);
                        render->drawIndexs(0, 3, false);
                        shader->setInt("drawfalg", 0);
                        render->drawIndexs(3, 3, false);
                        mVAO->unbind();
            */
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
            ui.drawCircle(UIPoint(100, 200), 60.0f, RGBA(255, 0, 0, 255), RGBA(0, 255, 0, 255), 12, 8);
            ui.fillCircle(UIPoint(100, 400), 60.0f, RGBA(255, 0, 0, 255), 32);
            ui.fillCircle(UIPoint(300, 400), 100.0f, RGBA(255, 0, 0, 255), RGBA(0, 255, 0, 255), 32);
            // 绘制纹理
            ui.drawRactTex(UIPoint(w / 3.0f, h / 3.0f), UIPoint(0, 1), UIPoint(w / 3.0f * 2, h / 3.0f * 2), UIPoint(1, 0));

            mVBO->setData(mDrawList.mVertices.data(), mDrawList.mVertices.size() * sizeof(UIVertex));
            auto icount = mDrawList.mIndices.size();
            auto isize = icount * sizeof(UIIndex);
            mIBO->setData(mDrawList.mIndices.data(), isize, icount);
            mVAO->bind();

            shader->setInt("drawflag", 1);
            render->drawIndexs(0, icount - 6, false);
            shader->setInt("drawflag", 0);
            mTexture->bind();
            // render->drawIndexs(icount - 6, 6, false);
            mVAO->unbind();
            // Sleep(100);
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