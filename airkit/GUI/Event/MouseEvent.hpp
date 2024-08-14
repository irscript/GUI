#ifndef __MOUSEEVENT_H__
#define __MOUSEEVENT_H__

#include <airkit/GUI/Event/IEvent.hpp>

namespace airkit
{
    enum class MouseAction : uint16_t
    {
        Unknow,

        Down,         // 按下：鼠标按键按下
        Click = Down, // 单击：鼠标按键按下
        Up,           // 抬起：鼠标按键抬起

        DblClk, // 双击：鼠标按键按下、抬起、按下，两个按下事件之间间隔小于系统定义的间隔时间

        Move,
        Wheel,

        Enter,
        Leave,

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
        IMouseEvent(MouseAction action, MouseButton button = MouseButton::Unknow)
            : IEvent(EventKind::Mouse),
              mAction(action), mButton(button) {}

        virtual ~IMouseEvent() = 0;
        MouseAction getAction() const { return mAction; }
        MouseButton getButton() const { return mButton; }

    protected:
        MouseAction mAction; // 鼠标动作
        MouseButton mButton; // 鼠标按键
    };
    // 鼠标按键事件
    struct IMouseButtonEvent : public IMouseEvent
    {
    public:
        IMouseButtonEvent(MouseAction action, MouseButton button, float x, float y)
            : IMouseEvent(action, button), mX(x), mY(y) {}

        virtual ~IMouseButtonEvent() = 0;

        float getX() const { return mX; }
        float getY() const { return mY; }

    protected:
        float mX;
        float mY;
    };

    // 鼠标单击事件
    struct MouseClickEvent : public IMouseButtonEvent
    {
    public:
        MouseClickEvent(MouseButton button, float x, float y)
            : IMouseButtonEvent(MouseAction::Click, button, x, y) {}

        virtual ~MouseClickEvent() = default;
    };
    // 鼠标双击事件
    struct MouseDblClkEvent : public IMouseButtonEvent
    {
    public:
        MouseDblClkEvent(MouseButton button, float x, float y)
            : IMouseButtonEvent(MouseAction::DblClk, button, x, y) {}

        virtual ~MouseDblClkEvent() = default;
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
            : IMouseEvent(MouseAction::Move), mX(x), mY(y) {}

        virtual ~MouseMoveEvent() = default;

        float getX() const { return mX; }
        float getY() const { return mY; }

    protected:
        float mX;
        float mY;
    };

    // 鼠标滚轮事件：X、Y 是水平和垂直方向的偏移量
    struct MouseWheelEvent : public IMouseEvent
    {
    public:
        MouseWheelEvent(float x, float y)
            : IMouseEvent(MouseAction::Wheel), mX(x), mY(y) {}

        virtual ~MouseWheelEvent() = default;

        float getX() const { return mX; }
        float getY() const { return mY; }

    protected:
        float mX;
        float mY;
    };

}
#endif // __MOUSEEVENT_H__