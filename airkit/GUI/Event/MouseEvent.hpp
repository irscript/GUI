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
    struct IMouseEvent : public IEvent
    {

        virtual ~IMouseEvent() = default;
        MouseAction getAction() const { return mAction; }

        float getX() const { return mX; }
        float getY() const { return mY; }

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

        virtual ~MouseUpEvent() = default;
    };

    // 鼠标移动事件：X、Y 是窗口坐标
    struct MouseMoveEvent : public IMouseEvent
    {
    public:
        MouseMoveEvent(float x, float y)
            : IMouseEvent(MouseAction::Move, x, y) {}

        virtual ~MouseMoveEvent() = default;
    };

    // 鼠标滚轮事件：X、Y 是水平和垂直方向的偏移量
    struct MouseWheelEvent : public IMouseEvent
    {
    public:
        MouseWheelEvent(float x, float y)
            : IMouseEvent(MouseAction::Wheel, x, y) {}

        virtual ~MouseWheelEvent() = default;
    };

    struct MouseEnterEvent : public IMouseEvent
    {
    public:
        MouseEnterEvent(float x, float y)
            : IMouseEvent(MouseAction::Enter, x, y) {}

        virtual ~MouseEnterEvent() = default;
    };
    struct MouseLeaveEvent : public IMouseEvent
    {
    public:
        MouseLeaveEvent(float x, float y)
            : IMouseEvent(MouseAction::Leave, x, y) {}

        virtual ~MouseLeaveEvent() = default;
    };
    struct MouseHoverEvent : public IMouseEvent
    {
    public:
        MouseHoverEvent(float x, float y)
            : IMouseEvent(MouseAction::Hover, x, y) {}

        virtual ~MouseHoverEvent() = default;
    };
}
#endif // __MOUSEEVENT_H__