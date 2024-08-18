#ifndef __UIEVENT_H__
#define __UIEVENT_H__
#include <airkit/GUI/Event/IEvent.hpp>
#include <airkit/GUI/UI/UIArea.hpp>

namespace airkit
{
    // UI动作包含窗口和UI元素的动作
    enum class UIAction : uint32_t
    {
        Unknow,

        Show,  // UI元素显示、隐藏事件
        Focus, // UI元素获得焦点、失去焦点事件

        Moving,   // UI元素移动正在移动
        Moved,    // UI元素移动完成移动
        Resizing, // UI元素正在调整大小
        Resized,  // UI元素调整大小完成

    };

    struct UIEvent : public IEvent
    {

        UIAction getAction() const { return mAction; }

    protected:
        UIEvent(UIAction action)
            : IEvent(EventKind::UI), mAction(action)
        {
        }
        virtual ~UIEvent() = default;

    private:
        UIAction mAction;
    };

    struct UIShowEvent : public UIEvent
    {
        UIShowEvent(bool isHide)
            : UIEvent(UIAction::Show), mIsHide(isHide) {}
        virtual ~UIShowEvent() = default;
        bool isHide() const { return mIsHide; }

    protected:
        bool mIsHide; // 是否隐藏
    };
    struct UIFocusEvent : public UIEvent
    {
        UIFocusEvent(bool isFocus)
            : UIEvent(UIAction::Focus), mIsFocus(isFocus) {}
        virtual ~UIFocusEvent() = default;
        bool isFocus() const { return mIsFocus; }

    protected:
        bool mIsFocus; // 是否获得焦点
    };

    struct UIMoveEvent : public UIEvent
    {

        virtual ~UIMoveEvent() = default;

    protected:
        UIMoveEvent(const UIPoint &pos, UIAction act)
            : UIEvent(act), mPos(pos) {}

        UIMoveEvent(float x, float y, UIAction act)
            : UIEvent(act), mPos(x, y) {}

    protected:
        UIPoint mPos;
    };

    struct UIMovingEvent : public UIMoveEvent
    {
        UIMovingEvent(const UIPoint &pos)
            : UIMoveEvent(pos, UIAction::Moving) {}

        UIMovingEvent(float x, float y)
            : UIMoveEvent(x, y, UIAction::Moving) {}
        UIPoint &getPos() { return mPos; }
        virtual ~UIMovingEvent() = default;
    };
    struct UIMovedEvent : public UIMoveEvent
    {
        UIMovedEvent(const UIPoint &pos)
            : UIMoveEvent(pos, UIAction::Moved) {}
        UIMovedEvent(float x, float y)
            : UIMoveEvent(x, y, UIAction::Moved) {}
        const UIPoint &getPos() const { return mPos; }
        virtual ~UIMovedEvent() = default;
    };

    struct UIResizeEvent : public UIEvent
    {
        virtual ~UIResizeEvent() = default;

    protected:
        UIResizeEvent(const UIArea &area, UIAction act)
            : UIEvent(UIAction::Resized), mArea(area) {}

    protected:
        UIArea mArea;
    };
    struct UIResizingEvent : public UIResizeEvent
    {
        UIResizingEvent(const UIArea &area)
            : UIResizeEvent(area, UIAction::Resizing) {}
        UIArea &getArea() { return mArea; }
        virtual ~UIResizingEvent() = default;
    };
    struct UIResizedEvent : public UIResizeEvent
    {
        UIResizedEvent(const UIArea &area)
            : UIResizeEvent(area, UIAction::Resized) {}
        const UIArea &getArea() const { return mArea; }
        virtual ~UIResizedEvent() = default;
    };
}

#endif // __UIEVENT_H__