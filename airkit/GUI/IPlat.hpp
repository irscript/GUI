#ifndef __IPLAT_H__
#define __IPLAT_H__

#include <airkit/GUI/WindowHub.hpp>
#include <airkit/GUI/Render/IRender.hpp>
#define FMT_HEADER_ONLY
#include <airkit/3Part/fmt/core.h>
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
        virtual UIHolder createWindow(uint32_t width, uint32_t height, const char *title, const UIHolder &shared) = 0;
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

// 检查错误
#define checkError(cond, ...)                                                                  \
    do                                                                                         \
    {                                                                                          \
        if (!((cond)))                                                                         \
        {                                                                                      \
            static constexpr char posfmt[] = "\nfunc: {0}\nfile: {1}\nline: {2}\ncond: {3}\n"; \
            std::string pos = fmt::format(posfmt, thisfuncarg(), thisfile(), thisline(), #cond);   \
            std::string logmsg = fmt::format(__VA_ARGS__);                                     \
            IPlat::getInstance().error(logmsg + pos);                                          \
        }                                                                                      \
    } while (0)

}

#endif // __IPLAT_H__