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

            // 窗口元素的标记位
            WindowClose = Bitop::bit<FlagType>(1),       // 窗口是否已经关闭
            WindowFocus = Bitop::bit<FlagType>(2),       // 窗口是否获得焦点
            WindowMinimize = Bitop::bit<FlagType>(3),    // 窗口是否最小化
            WindowMaximize = Bitop::bit<FlagType>(4),    // 窗口是否最大化
            WindowResize = Bitop::bit<FlagType>(5),      // 窗口是否可改变大小
            WindowFullscreen = Bitop::bit<FlagType>(6),  // 窗口是否全屏
            WindowAlwaysOnTop = Bitop::bit<FlagType>(7), // 窗口是否始终置顶
            WindowModal = Bitop::bit<FlagType>(8),       // 窗口是否为模态窗口
            WindowTransparent = Bitop::bit<FlagType>(9), // 窗口是否透明
            WindowHide = Bitop::bit<FlagType>(10),       // 窗口是否隐藏
            MouseEnter = Bitop::bit<FlagType>(11),       // 鼠标在窗口内
            MouseTrack = Bitop::bit<FlagType>(12),       // 跟踪鼠标中

            // UI元素的标记位

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