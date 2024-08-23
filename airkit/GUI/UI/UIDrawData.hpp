#ifndef __UIDRAWDATA_H__
#define __UIDRAWDATA_H__
#include <airkit/AIR_PCH.hpp>
#include <airkit/GUI/UI/UIPoint.hpp>
#include <airkit/GUI/UI/UIColor.hpp>
#include <airkit/GUI/UI/UIArea.hpp>
#include <airkit/GUI/Render/ITexture.hpp>

#include <airkit/Core/Bitop.hpp>
namespace airkit
{
    using UIIndex = uint16_t;

    // UI 数据顶点结构
    struct UIVertex
    {
        UIVertex(const UIPoint &xy, const UIPoint &uv, const RGBA &color)
            : mXY(xy), mUV(uv), mColor(color) {}
        UIVertex(const UIPoint &xy, const RGBA &color)
            : mXY(xy), mUV(0.0, 0.0), mColor(color) {}

        UIVertex() = default;

        UIPoint mXY; // 顶点坐标
        UIPoint mUV; // 顶点纹理坐标
        RGBA mColor; // 顶点颜色
    };

    // 绘制命令
    struct UIDrawCommand
    {
        UIArea mClipRect;       // 裁剪区域
        UIIndex mStartIndex;    // 顶点索引起始位置
        UIIndex mIndexCount;    // 顶点索引数量
        uint32_t mDrawFlag;     // 绘制标志
        TextureHolder mTexture; // 纹理

        void setClipRect(const UIArea &clipRect) { mClipRect = clipRect; }
        void setTexture(const TextureHolder &texture) { mTexture = texture; }
    };

    struct DrawFlag
    {
        enum : uint32_t
        {
            Clip = Bitop::bit<uint32_t>(0),    // 裁剪矩阵未改变
            Vertex = Bitop::bit<uint32_t>(1),  // 不使用纹理：使用顶点颜色
            Texture = Bitop::bit<uint32_t>(2), // 非字体纹理纹理：使用纹理颜色
            // 字体纹理的一些标记
        };
    };

    // 绘制列表
    struct UIDrawList
    {
        std::vector<UIVertex> mVertices;
        std::vector<UIIndex> mIndices;
        std::vector<UIDrawCommand> mDrawCommands;

        // 申请顶点内存缓存
        void needVertex(uint32_t count) { mVertices.reserve(mVertices.size() + count); }
        // 申请索引内存缓存
        void needIndex(uint32_t count) { mIndices.reserve(mIndices.size() + count); }

        // 开始：获取一个绘制命令
        UIDrawCommand &begin()
        {
            auto &cmd = mDrawCommands.emplace_back();
            cmd.mStartIndex = mIndices.size();
            return cmd;
        }
        // 结束：计算绘制命令的索引数量
        void end(UIDrawCommand &cmd, uint32_t flag)
        {
            cmd.mDrawFlag = flag;
            cmd.mIndexCount = mIndices.size() - cmd.mStartIndex;
        }
        // 清空命令
        void clear()
        {
            mDrawCommands.clear();
            mIndices.clear();
            mVertices.clear();
        }
    };

}

#endif // __UIDRAWDATA_H__