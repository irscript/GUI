#ifndef __MOUSEEVENT_H__
#define __MOUSEEVENT_H__

#include <airkit/GUI/Event/IEvent.hpp>

namespace airkit
{
    enum class MouseAction : uint16_t
    {
        Unknow,

        Down, // 按下：鼠标按键按下
        Up,   // 抬起：鼠标按键抬起

        Move,
        Wheel,

        Enter, // 鼠标进入
        Leave, // 鼠标离开
        Hover, // 鼠标停留

        Drag,
        Drop,

    };
    enum class MouseButton : uint16_t
    {
        Unknow,

        // From glfw3.h
        Button0,
        Button1,
        Button2,
        Button3,
        Button4,
        Button5,
        Button6,
        Button7,

        Last = Button7, // 最大值

        Left = Button0,  // 左键
        Right = Button1, // 右键
        Middle = Button2 // 中键
    };
    // 鼠标事件：X、Y 是UI坐标
    struct IMouseEvent : public IEvent
    {

        virtual ~IMouseEvent() = default;
        MouseAction getAction() const { return mAction; }

        float getX() const { return mX; }
        float getY() const { return mY; }

        UIPoint getCursorPos() const { return UIPoint(mX, mY); }

    protected:
        IMouseEvent(MouseAction action, float x, float y)
            : IEvent(EventKind::Mouse),
              mX(x), mY(y),
              mAction(action) {}

    protected:
        float mX; // 鼠标坐标x
        float mY; // 鼠标坐标y

        MouseAction mAction; // 鼠标动作
    };
    // 鼠标按键事件
    struct IMouseButtonEvent : public IMouseEvent
    {
    public:
        virtual ~IMouseButtonEvent() = default;
        MouseButton getButton() const { return mButton; }

    protected:
        IMouseButtonEvent(MouseAction action, MouseButton button, float x, float y)
            : IMouseEvent(action, x, y), mButton(button) {}

        MouseButton mButton; // 鼠标按键
    };

    // 鼠标单击事件
    struct MouseDownEvent : public IMouseButtonEvent
    {
    public:
        MouseDownEvent(MouseButton button, float x, float y, bool isDblClk = false)
            : IMouseButtonEvent(MouseAction::Down, button, x, y) {}
        MouseDownEvent(MouseButton button, const UIPoint &point, bool isDblClk = false)
            : MouseDownEvent(button, point.getX(), point.getY(), isDblClk) {}
        virtual ~MouseDownEvent() = default;

        bool isDblClk() const { return mIsDblClk; }

    protected:
        bool mIsDblClk = false;
    };
    // 鼠标抬起事件
    struct MouseUpEvent : public IMouseButtonEvent
    {
    public:
        MouseUpEvent(MouseButton button, float x, float y)
            : IMouseButtonEvent(MouseAction::Up, button, x, y) {}
        MouseUpEvent(MouseButton button, const UIPoint &point)
            : MouseUpEvent(button, point.getX(), point.getY()) {}
        virtual ~MouseUpEvent() = default;
    };

    // 鼠标移动事件
    struct MouseMoveEvent : public IMouseEvent
    {
    public:
        MouseMoveEvent(float x, float y)
            : IMouseEvent(MouseAction::Move, x, y) {}
        MouseMoveEvent(const UIPoint &point)
            : MouseMoveEvent(point.getX(), point.getY()) {}

        virtual ~MouseMoveEvent() = default;
    };

    // 鼠标滚轮事件：X、Y 是UI 坐标，delta 是滚动量，正数表示向上滚动，负数表示向下滚动

    struct MouseWheelEvent : public IMouseEvent
    {
    public:
        MouseWheelEvent(float x, float y, float delta)
            : IMouseEvent(MouseAction::Wheel, x, y), mDelta(delta) {}
        MouseWheelEvent(const UIPoint &point, float delta)
            : MouseWheelEvent(point.getX(), point.getY(), delta) {}

        virtual ~MouseWheelEvent() = default;

        float getDelta() const { return mDelta; }

    protected:
        float mDelta;
    };

    struct MouseEnterEvent : public IMouseEvent
    {
    public:
        MouseEnterEvent(float x, float y)
            : IMouseEvent(MouseAction::Enter, x, y) {}

        MouseEnterEvent(const UIPoint &point)
            : MouseEnterEvent(point.getX(), point.getY()) {}

        virtual ~MouseEnterEvent() = default;
    };
    struct MouseLeaveEvent : public IMouseEvent
    {
    public:
        MouseLeaveEvent(float x, float y)
            : IMouseEvent(MouseAction::Leave, x, y) {}
        MouseLeaveEvent(const UIPoint &point)
            : MouseLeaveEvent(point.getX(), point.getY()) {}
        virtual ~MouseLeaveEvent() = default;
    };
    struct MouseHoverEvent : public IMouseEvent
    {
    public:
        MouseHoverEvent(float x, float y)
            : IMouseEvent(MouseAction::Hover, x, y) {}
        MouseHoverEvent(const UIPoint &point)
            : MouseHoverEvent(point.getX(), point.getY()) {}
        virtual ~MouseHoverEvent() = default;
    };
}
#endif // __MOUSEEVENT_H__