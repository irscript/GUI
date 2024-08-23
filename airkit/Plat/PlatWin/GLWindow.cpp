#include <airkit/Plat/PlatWin/GLWindow.hpp>
#include <airkit/GUI/IPlat.hpp>
#include <airkit/Render/GL/GLRender.hpp>
#include "GLWindow.hpp"
namespace airkit
{
    void GLWindow::prepare()
    {
        wglMakeCurrent(mWinDC, mGLContext);
        auto &gl = getGlDriver();
        auto w = mArea.getWidth();
        auto h = mArea.getHeight();
        gl.Viewport(0, 0, w, h);
        gl.Scissor(0, 0, w, h);
    }

    void GLWindow::present()
    {
        SwapBuffers(mWinDC);
    }
    void GLWindow::render()
    {
        auto render = IPlat::getInstance().getRender();
        prepare();

        auto tick = GetTickCount();
        render->clearColor(tick % 3 / 3.0f, tick % 5 / 5.0f, tick % 7 / 7.0f, 1.0f);
        render->clear();
        present();
    }
    int32_t GLWindow::doModal()
    {
        auto render = IPlat::getInstance().getRender();
        wglMakeCurrent(mWinDC, mGLContext);
        // 创建着色器
        auto shader = render->createShader("default", "FlatColor.glsl");
        VertexLayout layout = {
            {"aPos", ShaderDataType::Float3},
        };
        // 创建管线
        auto pipeline = render->createPipeline("default", layout, shader);
        // 创建顶点数组
        auto vao = render->createVertexArray();
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
        auto vbo = render->createVertexBuffer(pipeline->getVertexLayout(), vertices, sizeof(vertices));
        // 创建索引缓冲
        auto ibo = render->createIndexBuffer(indices, sizeof(indices), 6);
        vao->addVertexBuffer(vbo);
        vao->setIndexBuffer(ibo);

        while (shouldClose() == false)
        {

            {
                MSG message = {0};
                BOOL result = GetMessageW(&message, 0, 0, 0);
                if (result > 0)
                {
                    TranslateMessage(&message);
                    DispatchMessageW(&message);
                }
            }
            prepare();

            render->clearColor(1.0f, 1.0f, 1.0f, 1.0f);
            render->clear();

            pipeline->bind();
            vao->bind();
            render->drawIndexs(0, 6, false);

            present();
        }
        return 0;
    }
    void airkit::GLWindow::onSized(UIResizedEvent &event)
    {
        mArea = event.getArea();
        render();
    }
}
