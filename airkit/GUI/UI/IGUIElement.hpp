#ifndef __IGUIELEMENT_H__
#define __IGUIELEMENT_H__

#include <airkit/GUI/Event/Event.hpp>
#include <airkit/GUI/UI/UIFlag.hpp>
#include <airkit/GUI/UI/UIArea.hpp>
namespace airkit
{
    struct IGUIElement;
    using UIHolder = std::shared_ptr<IGUIElement>;
    using UIWatcher = std::weak_ptr<IGUIElement>;
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
        UIHolder getParentUI() { return mParentUI.lock(); }
        const UIHolder getParentUI() const { return mParentUI.lock(); }

        // 获取窗口
        UIHolder getWindowUI();
        const UIHolder getWindowUI() const;

    protected:
        UIFlag mUIFlag;               // UI 标志
        UIArea mArea;                 // UI 区域位置大小
        UIWatcher mParentUI;          // 父 UI
        std::list<UIHolder> mChildUI; // 子 UI
    };

}

#endif // __IGUIELEMENT_H__