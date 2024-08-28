#ifndef __IEVENT_H__
#define __IEVENT_H__

#include <airkit/AIR_PCH.hpp>

namespace airkit
{
    // 事件类别枚举
    enum class EventKind : uint32_t
    {
        Unknown,
        User,      // 用户事件
        Mouse,     // 鼠标事件
        Keyboard,  // 键盘事件
        CharInput, // 字符输入事件
        UI,        // UI事件，窗口是特殊的UI
    };

    // 事件基类
    struct IEvent
    {
        virtual ~IEvent() = 0;

        EventKind getKind() const { return mKind; }

        template <typename T>
        const T &as() const { return *(T *)(this); }
        template <typename T>
        T &as() { return *(T *)(this); }

        // 是否已处理
        bool isHandled() const { return mHandled; }
        // 设置已处理
        void setHandled() { mHandled = true; }

    protected:
        IEvent(EventKind kind) : mKind(kind) {}

    private:
        EventKind mKind;
        bool mHandled = false; // 是否处理
    };

    struct CharInputEvent : public IEvent
    {
        CharInputEvent(uint32_t utf32) : IEvent(EventKind::CharInput), mUtf32(utf32) {}
        virtual ~CharInputEvent() = default;
        uint32_t getUtf32() const { return mUtf32; }

    protected:
        uint32_t mUtf32;
    };

}

#endif // __IEVENT_H__