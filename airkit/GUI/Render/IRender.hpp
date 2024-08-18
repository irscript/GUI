#ifndef __IRENDER_H__
#define __IRENDER_H__

#include <airkit/GUI/Render/RenderEnum.hpp>
#include <airkit/GUI/Render/IPipeline.hpp>
#include <airkit/GUI/Render/IBuffer.hpp>

namespace airkit
{

    struct IRender
    {
        virtual ~IRender() = 0;

        virtual RenderAPI getAPI() const = 0;
        virtual const char *getAPIName() const = 0;

        virtual bool init(void *data = nullptr) = 0;
        virtual void shutdown() = 0;

        virtual void clear() = 0;
        // 清空颜色
        virtual void clearColor(float r, float g, float b, float a) = 0;
        // 设置设置视口
        virtual void setViewport(int x, int y, int width, int height) = 0;
        // 设置裁剪区域
        virtual void setScissor(int x, int y, int width, int height) = 0;

        // 创建着色器
        virtual ShaderWatcher createShader(const std::string &name, const std::string &vertex, const std::string &fragment, bool isfile = true) = 0;
        virtual ShaderWatcher createShader(const std::string &name, const std::string &src, bool isfile = true) = 0;
        // 获取着色器
        ShaderWatcher get(const std::string &name) { return mShaderLibrary.get(name); }

        // 创建渲染管线
        virtual PipelineHolder createPipeline(const std::string &name, const VertexLayout &layout, const ShaderWatcher &shader) = 0;

        // 创建顶点缓存
        virtual VBOHolder createVertexBuffer(const VertexLayout &layout, const void *data, uint32_t size) = 0;
        // 创建索引缓存
        virtual IBOHolder createIndexBuffer(const void *data, uint32_t size, uint32_t count) = 0;
        // 创建顶点数组
        virtual VAOHolder createVertexArray() = 0;

        // 索引绘制
        virtual void drawIndexs(uint32_t offset, uint32_t count, bool isI32) = 0;
        // 顶点绘制
        virtual void drawVertices(uint32_t offset, uint32_t count) = 0;

    protected:
        ShaderLibrary mShaderLibrary;
    };

    using RenderHolder = std::shared_ptr<IRender>;
    using RenderWatcher = std::weak_ptr<IRender>;
}

#endif // __IRENDER_H__