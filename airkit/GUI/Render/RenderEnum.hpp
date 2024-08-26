#ifndef __RENDERENUM_H__
#define __RENDERENUM_H__

#include <airkit/AIR_PCH.hpp>
namespace airkit
{
    // 渲染API标识
    enum class RenderAPI : uint16_t
    {
        Unknown,

        OpenGL,
    };
    // 着色器阶段
    enum class ShaderStage : uint16_t
    {
        Unknown,
        Vertex,
        Fragment,
    };
    // 着色器数据类型
    enum class ShaderDataType : uint16_t
    {
        Unknown = 0,
        Float,
        Float2,
        Float3,
        Float4,
        Mat3,
        Mat4,
        Int,
        Int2,
        Int3,
        Int4,
        Bool
    };

    // 颜色格式
    enum class ColorFormat : uint16_t
    {
        Unknown,
        R8,
        RGB24,
        RGBA32,
    };

    // 文本绘制标志
    struct TextFlag
    {
        enum : uint32_t
        {
            None = 0,

            Top = 1 << 0,     // 顶部对齐
            VCenter = 1 << 1, // 垂直居中
            Bottom = 1 << 2,  // 底部对齐

            Left = 1 << 3,    // 左对齐
            HCenter = 1 << 4, // 水平居中
            Right = 1 << 5,   // 右对齐

            Wrap = 1 << 6, // 换行

            Strikeout = 1 << 7, // 删除线
            Underline = 1 << 8, // 下划线

            // 默认值
            Default = Top | HCenter

        };
    };

}
#endif // __RENDERENUM_H__