#ifndef __IEVENT_H__
#define __IEVENT_H__

#include <airkit/AIR_PCH.hpp>

namespace airkit
{
    // 事件类别枚举
    enum class EventKind : uint32_t
    {
        Unknow,
        User,     // 用户事件
        Window,   // 窗口事件
        Mouse,    // 鼠标事件
        Keyboard, // 键盘事件
        Text,     // 文本事件
    };

    // 事件基类
    struct IEvent
    {
        virtual ~IEvent() = 0;
        IEvent(EventKind kind) : mKind(kind) {}

        EventKind getKind() const { return mKind; }

    private:
        EventKind mKind;
    };
}

#endif // __IEVENT_H__