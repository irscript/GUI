#ifndef __IRENDER_H__
#define __IRENDER_H__

#include <airkit/GUI/Render/RenderEnum.hpp>
#include <airkit/GUI/Render/IShader.hpp>
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
        virtual ShaderWatcher createShader(const std::string &name, const std::string &vertex, const std::string &fragment,bool isfile=true) = 0;
        virtual ShaderWatcher createShader(const std::string &name, const std::string &file) = 0;
        // 获取着色器
        ShaderWatcher get(const std::string &name) { return mShaderLibrary.get(name); }

    protected:
        ShaderLibrary mShaderLibrary;
    };

    using RenderHolder = std::shared_ptr<IRender>;
    using RenderWatcher = std::weak_ptr<IRender>;
}

#endif // __IRENDER_H__