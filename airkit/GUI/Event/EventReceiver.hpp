#ifndef __EVENTRECEIVER_H__
#define __EVENTRECEIVER_H__
#include <airkit/GUI/Event/Event.hpp>
namespace airkit
{
    // 事件接收者：可用于事件过滤
    struct IEventReceiver
    {
        virtual ~IEventReceiver() = default;
        virtual void onEvent(IEvent &event) = 0;
    };
}
#endif // __EVENTRECEIVER_H__