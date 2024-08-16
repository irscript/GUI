#ifndef __WINDOWEVENT_H__
#define __WINDOWEVENT_H__

#include <airkit/GUI/Event/IEvent.hpp>
#include <airkit/GUI/UI/UIArea.hpp>

namespace airkit
{
    enum class WindowAction : uint32_t
    {
        Unknow,
        Focused,
        Unfocused,
        Closed,
        Minimized,
        Maximized,
        Restored,
        Moved,
        Resized,
        Shown,
        Hidden,

    };

    struct IWindowEvent : public IEvent
    {
        IWindowEvent(WindowAction action)
            : IEvent(EventKind::Window), mAction(action) {}

        virtual ~IWindowEvent() = 0;
        WindowAction getAction() const { return mAction; }

    private:
        WindowAction mAction;
    };

    // 窗口焦点事件
    struct WindowFocusEvent final : public IWindowEvent
    {
        WindowFocusEvent(bool focused)
            : IWindowEvent(focused ? WindowAction::Focused : WindowAction::Unfocused) {}
        virtual ~WindowFocusEvent() = default;

        bool isFocused() const { return getAction() == WindowAction::Focused; }
    };
    // 窗口移动事件
    struct WindowMoveEvent final : public IWindowEvent
    {
        WindowMoveEvent(float x, float y)
            : IWindowEvent(WindowAction::Moved), mX(x), mY(y) {}
        virtual ~WindowMoveEvent() = default;

        float getX() const { return mX; }
        float getY() const { return mY; }

    private:
        float mX;
        float mY;
    };

    // 窗口大小改变事件
    struct WindowResizeEvent final : public IWindowEvent
    {
        WindowResizeEvent(float x, float y, float width, float height)
            : IWindowEvent(WindowAction::Resized), mArea(x, y, width, height) {}
        virtual ~WindowResizeEvent() = default;

        UIArea getArea() const { return mArea; }

    private:
        UIArea mArea;
    };

}
#endif // __WINDOWEVENT_H__