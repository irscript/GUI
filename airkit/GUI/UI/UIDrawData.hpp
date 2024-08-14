#ifndef __UIDRAWDATA_H__
#define __UIDRAWDATA_H__
#include <airkit/AIR_PCH.hpp>
#include <airkit/GUI/UI/UIPoint.hpp>
#include <airkit/GUI/UI/UIColor.hpp>
#include <airkit/GUI/UI/UIArea.hpp>
namespace airkit
{
    using UIIndex = uint16_t;

    // UI 数据顶点结构
    struct UIVertex
    {
        UIPoint mXY;    // 顶点坐标
        UIPoint mUV;    // 顶点纹理坐标
        RGBA mColor;    // 顶点颜色
        uint32_t mFlag; // 顶点标志
    };

    // 绘制命令
    struct UIDrawCommand
    {
        UIArea mClipRect;    // 裁剪区域
        UIIndex mStartIndex; // 顶点索引起始位置
        UIIndex mIndexCount; // 顶点索引数量
    };

    // 绘制列表
    struct UIDrawList
    {
        std::vector<UIVertex> mVertices;
        std::vector<UIIndex> mIndices;
        std::vector<UIDrawCommand> mDrawCommands;
    };
}

#endif // __UIDRAWDATA_H__