#ifndef __UIFLAG_H__
#define __UIFLAG_H__

#include <airkit/Core/Bitop.hpp>

namespace airkit
{

    // UI元素的一些标记位
    struct UIFlag : public Bit32
    {
        using FlagType = uint32_t;

    public:
        enum : FlagType
        {
            Window = Bitop::bit<FlagType>(0), // 用于识别UI元素是否为窗口

            Enabled = Bitop::bit<FlagType>(1),   // 用于识别UI元素是否可用
            Movable = Bitop::bit<FlagType>(2),   // 用于识别UI元素是否可移动
            Draggable = Bitop::bit<FlagType>(3), // 用于识别UI元素是否可拖动
            Drawable = Bitop::bit<FlagType>(4),  // 用于识别UI元素是否可绘制
            Visible = Bitop::bit<FlagType>(5),   // 用于识别UI元素是否可见

            Closed = Bitop::bit<FlagType>(6),  // 用于识别UI元素是否应该关闭
            Focused = Bitop::bit<FlagType>(7), // 用于识别UI元素是否已获得焦点

            Text = Bitop::bit<FlagType>(8), // 用于识别UI元素是否为文本

        };
    };
}

#endif // __UIFLAG_H__