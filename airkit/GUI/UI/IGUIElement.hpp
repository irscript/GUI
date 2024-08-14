#ifndef __IGUIELEMENT_H__
#define __IGUIELEMENT_H__

#include <airkit/GUI/Event/Event.hpp>
#include <airkit/GUI/UI/UIFlag.hpp>
#include <airkit/GUI/UI/UIArea.hpp>
namespace airkit
{
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
        using UIPtr = std::shared_ptr<IGUIElement>;

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
    struct IGUIWidget : public IGUIElement
    {
        virtual ~IGUIWidget() = 0;
    };
}

#endif // __IGUIELEMENT_H__