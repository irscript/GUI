#ifndef __IGUIELEMENT_H__
#define __IGUIELEMENT_H__

#include <airkit/GUI/Event/Event.hpp>
#include <airkit/GUI/UI/UIFlag.hpp>
#include <airkit/GUI/UI/UIArea.hpp>
#include <airkit/GUI/UI/UIDrawData.hpp>
#include <airkit/GUI/Theme/UITheme.hpp>
namespace airkit
{
    struct IGUIElement;
    using UIHolder = Holder<IGUIElement>;
    using UIWatcher = Watcher<IGUIElement>;

    // UI 环境
    struct UIVibe
    {
        IGUIElement *mHover; // 鼠标悬浮下的UI
        IGUIElement *mFocus; // 获取焦点的UI

        double mDiffTime;
        double mPrevTime;
        double mCurrTime;
    };

    // UI 元素基类
    struct IGUIElement
    {
    public:
        IGUIElement() = default;
        virtual ~IGUIElement() = 0;

        template <typename T>
        const T &as() const { return *(T *)(this); }
        template <typename T>
        T &as() { return *(T *)(this); }

    public:
        // 事件响应
        virtual void onEvent(IEvent &event);

        // 响应焦点变化
        virtual void onFocus(UIFocusEvent &event);
        // 响应显示状态变化
        virtual void onShow(UIShowEvent &event);

        // 响应大小变化
        virtual void onSizing(UIResizingEvent &event);
        virtual void onSized(UIResizedEvent &event);
        // 响应位置变化
        virtual void onMoving(UIMovingEvent &event);
        virtual void onMoved(UIMovedEvent &event);

        // 响应字符输入
        virtual void onCharInput(CharInputEvent &event);

        // 响应键盘事件
        virtual void onKeyDown(KeyDownEvent &event);
        virtual void onKeyUp(KeyUpEvent &event);

        // 响应鼠标事件
        virtual void onMouseClick(MouseDownEvent &event);
        virtual void onMouseUp(MouseUpEvent &event);
        virtual void onMouseMove(MouseMoveEvent &event);
        virtual void onMouseEnter(MouseEnterEvent &event);
        virtual void onMouseLeave(MouseLeaveEvent &event);
        virtual void onMouseHover(MouseHoverEvent &event);
        virtual void onMouseWheel(MouseWheelEvent &event);

        // 响应命中测试
        virtual IGUIElement *onHitTest(const UIHitEvent &event);

    public:
        // 响应主题变化
        virtual void onThemeChange(UITheme &event);
        // 渲染帧
        virtual void onRenderFrame(const UIVibe &vibe, const UIArea &clip, UIDrawList &drawList);

    public:
        // 坐标转换

        /// @brief 屏幕坐标转窗口坐标
        /// @param point 屏幕坐标值
        void screenToWindow(UIPoint &point) const;
        /// @brief 窗口坐标转屏幕坐标
        /// @param point 屏幕坐标值
        void windowToScreen(UIPoint &point) const;

        /// @brief UI坐标转窗口坐标
        /// @param point UI坐标值
        void UIToWindow(UIPoint &point) const;
        /// @brief 窗口坐标转UI坐标
        /// @param point 窗口坐标值
        void windowToUI(UIPoint &point) const;

        /// @brief UI坐标转窗口坐标
        /// @param point 屏幕坐标值
        void UIToscreen(UIPoint &point) const;
        /// @brief 屏幕坐标转UI坐标
        /// @param point 屏幕坐标值
        void screenToUI(UIPoint &point) const;

        uint32_t getUIFlag(uint32_t mask) const { return mUIFlag.getValue() & mask; }
        void setUIFlag(uint32_t mask) { mUIFlag.setMask(mask); }
        void resetUIFlag(uint32_t mask) { mUIFlag.resetMask(mask); }
        void flipUIFlag(uint32_t mask) { mUIFlag.flipMask(mask); }

    public:
        // 获取父UI
        UIHolder getUIParent() { return mParentUI.lock(); }
        const UIHolder getUIParent() const { return mParentUI.lock(); }

        void setUIParent(UIHolder parent) { mParentUI = parent; }

        // 获取窗口
        UIHolder getUIWindow();
        const UIHolder getUIWindow() const;
        // 设置UI大小限制的最值
        virtual void setUILimit(const UILimit &limit);
        const UILimit &getUILimit() const { return mLimit; }
        UILimit &getUILimit() { return mLimit; }

        // 获取UI区域
        const UIArea &getArea() const { return mArea; }
        UIArea &getArea() { return mArea; }

        // 获取自身的持有者
        virtual UIHolder getSelf();
        // 获取UI持有者
        virtual UIHolder getHolder(IGUIElement *ui) { return UIHolder(); }

    protected:
        UIFlag mUIFlag;      // UI 标志
        UIArea mArea;        // UI 区域位置大小
        UIWatcher mParentUI; // 父 UI

        UILimit mLimit; // UI 大小限制的最值
    };

    // 有子元素的UI基类
    struct IUIParent : public IGUIElement
    {
        // 响应命中测试
        virtual IGUIElement *onHitTest(const UIHitEvent &event) override;
        // 渲染帧
        virtual void onRenderFrame(const UIVibe &vibe, const UIArea &clip, UIDrawList &drawList) override;
        virtual UIHolder getHolder(IGUIElement *ui) override;

        // 添加子UI
        void addChild(UIHolder ui) { mChildUI.push_back(ui); }
        // 移除子UI
        void removeChild(UIHolder ui) { mChildUI.remove(ui); }

    protected:
        std::list<UIHolder> mChildUI; // 子 UI
    };
    // 布局基类
    struct ILayout
    {
        virtual void onLayout(UIArea &area) = 0;
    };
}

#endif // __IGUIELEMENT_H__