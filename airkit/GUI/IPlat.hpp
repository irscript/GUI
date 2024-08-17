#ifndef __IPLAT_H__
#define __IPLAT_H__

#include <airkit/GUI/WindowHub.hpp>
#include <airkit/GUI/Render/IRender.hpp>
namespace airkit
{

    // 一些平台接口
    struct IPlat
    {
        IPlat() : mRenderAPI() {}

        virtual ~IPlat() = 0;
        static IPlat &getInstance() { return *mInstance; }

        // 获取窗口管理器
        const WindowHub &getWinHub() const { return mWinHub; }
        // 创建窗口
        virtual UIHolder createWindow(uint32_t width, uint32_t height, const char *title) = 0;
        // 释放窗口
        virtual void releaseWindow(const IGUIElement *win) = 0;

        // 获取渲染接口API
        RenderAPI getRenderAPI() const { return mRenderAPI; }
        RenderHolder getRender() { return mRender; }

        virtual void init(RenderAPI api) = 0;
        virtual void shutdown() = 0;

        // 错误信息处理
        virtual void error(const std::string &msg) = 0;
        virtual void warning(const std::string &msg) = 0;

    protected:
        WindowHub mWinHub;    // 窗口管理器
        RenderAPI mRenderAPI; // 渲染接口API
        RenderHolder mRender; // 渲染接口
    protected:
        static IPlat *mInstance;
    };

}

#endif // __IPLAT_H__