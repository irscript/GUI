#ifndef __IWINDOW_H__
#define __IWINDOW_H__

#include <airkit/GUI/UI/IGUIElement.hpp>

namespace airkit
{
    struct IWindow : public IGUIElement
    {
        IWindow() : IGUIElement() { mUIFlag.setMask(UIFlag::Window); }

        virtual ~IWindow() = 0;

        void setShouldClose(bool shouldClose);
        bool shouldClose() const;
        bool isFullScreen() const;

        virtual void prepare() = 0; // 渲染准备：切换上下文等
        virtual void present() = 0; // 渲染完成：显示结果等
        virtual void render() = 0;  // 渲染一帧

        virtual UIPoint getCursorPos() = 0; // 获取鼠标位置，窗口坐标

        virtual int32_t doModal() = 0;

        virtual void maximize() = 0; // 最大化
        virtual void restore() = 0;  // 还原
        virtual void minimize() = 0; // 最小化
        virtual void close() = 0;    // 关闭

        UIVibe &getUIVibe() { return mUIVibe; }
        const UIVibe &getUIVibe() const { return mUIVibe; }

        // 设置标题栏
        virtual void setTitleBar(UIHolder bar) = 0;
        UIHolder setTitleBar(IGUIElement *bar)
        {
            setTitleBar(bar);
            return mTitleBar;
        }
        UIHolder getTitleBar() const { return mTitleBar; }
        // 设置内容
        virtual void setContent(UIHolder content) = 0;
        UIHolder setContent(IGUIElement *bar)
        {
            setContent(bar);
            return mContent;
        }
        UIHolder getContent() const { return mContent; }

        // 获取UI持有者
        virtual UIHolder getHolder(IGUIElement *ui)
        {
            if (mTitleBar.get() == ui)
                return mTitleBar;
            if (mContent.get() == ui)
                return mContent;
            return UIHolder();
        }

    protected:
        UIVibe mUIVibe;
        UIHolder mTitleBar; // 标题栏
        UIHolder mContent;  // 内容
    };
}
#endif // __IWINDOW_H__