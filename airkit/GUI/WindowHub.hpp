#ifndef __WINDOWHUB_H__
#define __WINDOWHUB_H__

#include <airkit/GUI/UI/IWindow.hpp>

namespace airkit
{
    // 窗口管理器
    struct WindowHub
    {
        // 查找窗口
        UIHolder findWindow(const IGUIElement *window) const { return mWinMap.find(window)->second; }
        // 添加窗口
        UIHolder addWindow(IWindow *window)
        {
            auto res = mWinMap.insert({window, UIHolder{window}});
            return res.first->second;
        }
        // 移除窗口
        void removeWindow(UIHolder winptr) { mWinMap.erase(winptr.get()); }
        void removeWindow(const IGUIElement *winptr) { mWinMap.erase(winptr); }

    protected:
        std::map<const IGUIElement *, UIHolder> mWinMap; // 窗口指针和窗口共享指针的映射
    };
}

#endif // __WINDOWHUB_H__