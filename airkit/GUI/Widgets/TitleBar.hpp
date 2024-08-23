#ifndef __TITLEBAR_H__
#define __TITLEBAR_H__

#include <airkit/GUI/Widgets/Button.hpp>

namespace airkit
{

    struct TitleBar : public IGUIElement
    {
    public:
        TitleBar() = default;
        virtual ~TitleBar() = default;

        // 响应命中测试
        virtual IGUIElement *onHitTest(const UIHitEvent &event);

        virtual void onRenderFrame(const UIVibe &vibe, const UIArea &clip, UIDrawList &drawList);

    protected:
        UIHolder mUIIcon;  // 窗口图标
        UIHolder mUITitle; // 窗口标题

        UIArea mBtnArea;      // 按钮区域
        UIHolder mUIClose;    // 关闭按钮
        UIHolder mUIMaximize; // 最大化按钮
        UIHolder mUIMinimize; // 最小化按钮

        UIArea mUseArea;     // 用户可用区域
        UIHolder mUIUseArea; // 用户可用区域UI

    public:
        // 标题栏图标
        struct ButtonIcon : public Button
        {
        public:
            ButtonIcon(const UIPoint &pos, const UISize &size) : Button(pos, size) {}
            ~ButtonIcon() = default;

            virtual void onRenderFrame(const UIVibe &vibe, const UIArea &clip, UIDrawList &drawList);
        };
        // 最大化
        struct ButtonMaximize : public Button
        {
        public:
            ButtonMaximize(const UIPoint &pos, const UISize &size) : Button(pos, size) {}
            ~ButtonMaximize() = default;

            virtual void onRenderFrame(const UIVibe &vibe, const UIArea &clip, UIDrawList &drawList);
            virtual void onMouseUp(MouseUpEvent &event);

        protected:
            bool mIsMaximize = false;
        };
        // 最小化
        struct ButtonMinimize : public Button
        {
        public:
            ButtonMinimize(const UIPoint &pos, const UISize &size) : Button(pos, size) {}
            ~ButtonMinimize() = default;

            virtual void onRenderFrame(const UIVibe &vibe, const UIArea &clip, UIDrawList &drawList);
            virtual void onMouseUp(MouseUpEvent &event);
        };
        // 关闭
        struct ButtonClose : public Button
        {
        public:
            ButtonClose(const UIPoint &pos, const UISize &size) : Button(pos, size) {}
            ~ButtonClose() = default;

            virtual void onRenderFrame(const UIVibe &vibe, const UIArea &clip, UIDrawList &drawList);
            virtual void onMouseUp(MouseUpEvent &event);
        };
    };

}

#endif // __TITLEBAR_H__