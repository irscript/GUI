#ifndef __IGUIELEMENT_H__
#define __IGUIELEMENT_H__

#include <airkit/GUI/Event/Event.hpp>
#include <airkit/GUI/UI/UIFlag.hpp>
#include <airkit/GUI/UI/UIArea.hpp>
namespace airkit
{
    struct IGUIElement;
    using UIPtr = std::shared_ptr<IGUIElement>;
    // UI 元素基类
    struct IGUIElement
    {
    public:
        IGUIElement() = default;
        virtual ~IGUIElement() = 0;

    public:
        // 事件响应
        virtual void onEvent(const IEvent &event) = 0;

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

    public:
        // 获取父UI
        UIPtr getParentUI() { return mParentUI; }
        const UIPtr getParentUI() const { return mParentUI; }

        // 获取窗口
        UIPtr getWindowUI();
        const UIPtr getWindowUI() const;

    protected:
        UIFlag mUIFlag;            // UI 标志
        UIArea mArea;              // UI 区域位置大小
        UIPtr mParentUI;           // 父 UI
        std::list<UIPtr> mChildUI; // 子 UI
    };

    // UI 布局元素基类：一般不会绘制，只响应布局操作
    struct IGUILayout : public IGUIElement
    {
        IGUILayout() : IGUIElement() {}
        virtual ~IGUILayout() = 0;

        // 响应布局改变，子类实现真正的布局算法
        virtual void onLayout(const float width, const float height) = 0;
    };

    // UI部件基类：
    struct IGUIVisable : public IGUIElement
    {
        virtual ~IGUIVisable() = 0;

        // virtual void onMove(const UIPoint &pos) = 0;
        // virtual void onFocus(bool focus) = 0;
    };

    // UI部件基类：
    struct IGUIWidget : public IGUIVisable
    {
        virtual ~IGUIWidget() = 0;
    };

}

#endif // __IGUIELEMENT_H__