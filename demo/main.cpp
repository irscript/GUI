#include <cstdio>
#include <airkit/Plat/PlatWin/PlatWin.hpp>
#include <airkit/Plat/PlatWin/GLWindow.hpp>
#include <airkit/GUI/UI/UIColor.hpp>
#include <airkit/GUI/UI/UIHelper.hpp>

#include <airkit/GUI/Theme/UIThemeLoader.hpp>
#include <airkit/GUI/Font/BmpFont.hpp>

using namespace airkit;

struct TitleBar : public WinTitleBar
{
    TitleBar() = default;
    void create(UIHolder parent)
    {
        mUIIcon = UIHolder{new TitleBar::ButtonIcon({}, {})};
        mUIIcon->setUIParent(parent);
        mUIClose = UIHolder{new TitleBar::ButtonClose({}, {})};
        mUIClose->setUIParent(parent);
        mUIMaximize = UIHolder{new TitleBar::ButtonMaximize({}, {})};
        mUIMaximize->setUIParent(parent);
        mUIMinimize = UIHolder{new TitleBar::ButtonMinimize({}, {})};
        mUIMinimize->setUIParent(parent);
    }
};

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

        mTexture = render->createTexture2D("bug.png");

        auto res2 = mFont.load("demo.bfnt", render);

        auto tbar = new ::TitleBar();
        UIHolder title(tbar);
        tbar->create(title);
        setTitleBar(title);
    }
    void onRenderFrame(const UIVibe &vibe, const UIArea &clip, UIDrawList &drawList) override
    {
        mTitleBar->onRenderFrame(vibe, clip, drawList);

        UIArea area = clip;
        area.mY = mTitleBar->getArea().getHeight();
        area.mHeight -= area.mY;

        mFont.drawText(drawList, area, "Hello World!", 0, RGBA(0,255,0), 32.0);
        
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

            mCmdlist.clear();

            UIArea clip(0, 0, mArea.getWidth(), mArea.getHeight());

            onRenderFrame(mUIVibe, clip, mCmdlist);

            // 没有数据
            if (mCmdlist.mVertices.size() == 0)
            {
                present();
                return;
            }
            render->clearColor(1.0f, 1.0f, 1.0f, 1.0f);
            render->clear();

            mPipeline->bind(); // 先生成数据
            auto wsize = mArea.getSize();

            float ubo[4];
            ubo[0] = 2.0f / wsize.getWidth();
            ubo[1] = 2.0f / wsize.getHeight();
            ubo[2] = -1.0f; //-wsize.getWidth() / 2.0f * ubo[0];
            ubo[3] = -1.0f; //-wsize.getHeight() / 2.0f * ubo[1];

            auto shader = mPipeline->getShader();
            shader->setFloat2("uScale", ubo);
            shader->setFloat2("uTranslate", ubo + 2);

            mVBO->setData(mCmdlist.mVertices.data(), mCmdlist.mVertices.size() * sizeof(UIVertex));
            auto icount = mCmdlist.mIndices.size();
            auto isize = icount * sizeof(UIIndex);
            mIBO->setData(mCmdlist.mIndices.data(), isize, icount);
            mVAO->bind();

            uint32_t uiflag = 0;
            shader->setInt("uiflag", 0);
            ITexture*tex=nullptr;
            for (auto &cmd : mCmdlist.mDrawCommands)
            {
                if (cmd.mDrawFlag != uiflag)
                {
                    uiflag = cmd.mDrawFlag;
                    shader->setInt("uiflag", uiflag);
                }
                if (cmd.mTexture.get() != tex)
                {
                    tex = cmd.mTexture.get();
                    tex->bind();
                }
                render->drawIndexs(cmd.mStartIndex, cmd.mIndexCount, false);
            }
            mCmdlist.clear();

            mVAO->unbind();
            present();
        }
    }

private:
    PipelineHolder mPipeline;
    VAOHolder mVAO;
    VBOHolder mVBO;
    IBOHolder mIBO;
    TextureHolder mTexture;
    UIDrawList mCmdlist; // 绘制命令列表
    BmpFont mFont;
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
    UITheme theme;
    UIThemeXmlPaser parser;

    auto res = parser.parse("theme.xml", theme);

    Plat plat;
    plat.init(RenderAPI::OpenGL);

    auto win = plat.createWindow(800, 600, "win", {});


    Window *winptr = (Window *)win.get();
    winptr->init("UIDraw.glsl");

    while (winptr->shouldClose() == false)
    {
        winptr->render();
    }

    plat.shutdown();

    return 0;
}