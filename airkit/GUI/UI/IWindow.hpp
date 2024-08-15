#ifndef __IWINDOW_H__
#define __IWINDOW_H__

#include <airkit/GUI/UI/IGUIElement.hpp>

namespace airkit
{
    struct IWindow : public IGUIVisable
    {
        IWindow() : IGUIVisable() { mUIFlag.set(UIFlag::Window); }

        virtual ~IWindow() = 0;

        void setShouldClose(bool shouldClose);
        bool shouldClose() const;

        virtual void prepare() = 0; // 渲染准备：切换上下文等
        virtual void present() = 0; // 渲染完成：显示结果等

        virtual int32_t doModal() = 0;
    };
}
#endif // __IWINDOW_H__