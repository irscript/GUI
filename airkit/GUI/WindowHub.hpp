#ifndef __WINDOWHUB_H__
#define __WINDOWHUB_H__

#include <airkit/GUI/UI/IWindow.hpp>

namespace airkit
{
    // 窗口管理器
    struct WindowHub
    {
        // 查找窗口
        UIPtr findWindow(const IGUIElement *window) const { return mWinMap.find(window)->second; }
        // 添加窗口
        UIPtr addWindow(IWindow *window)
        {
            auto res = mWinMap.insert({window, UIPtr{window}});
            return res.first->second;
        }
        // 移除窗口
        void removeWindow(UIPtr winptr) { mWinMap.erase(winptr.get()); }
        void removeWindow(const IGUIElement *winptr) { mWinMap.erase(winptr); }

    protected:
        std::map<const IGUIElement *, UIPtr> mWinMap; // 窗口指针和窗口共享指针的映射
    };
}

#endif // __WINDOWHUB_H__