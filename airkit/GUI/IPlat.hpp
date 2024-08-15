#ifndef __IPLAT_H__
#define __IPLAT_H__

#include <airkit/GUI/WindowHub.hpp>
#include <airkit/GUI/Render/IRender.hpp>
namespace airkit
{

    // 一些平台接口
    struct IPlat
    {
        IPlat(RenderAPI api): mRenderAPI(api) {}
        
        virtual ~IPlat() = 0;
        static IPlat &getInstance() { return *mInstance; }

        // 获取窗口管理器
        const WindowHub &getWinHub() const { return mWinHub; }

        // 获取渲染接口API
        RenderAPI getRenderAPI() const { return mRenderAPI; }

    protected:
        static IPlat *mInstance;

    protected:
        WindowHub mWinHub;    // 窗口管理器
        RenderAPI mRenderAPI; // 渲染接口API
    };

}

#endif // __IPLAT_H__