#ifndef __RENDERENUM_H__
#define __RENDERENUM_H__

#include <airkit/AIR_PCH.hpp>
namespace airkit
{
    // 渲染API标识
    enum class RenderAPI : uint16_t
    {
        Unknow,

        OpenGL,
    };
    // 着色器阶段
    enum class ShaderStage : uint16_t
    {
        Unknow,
        Vertex,
        Fragment,
    };
    // 着色器数据类型
    enum class ShaderDataType : uint16_t
    {
        Unknow = 0,
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
        Unknow,
        R8,
        RGB24,
        RGBA32,
    };
}
#endif // __RENDERENUM_H__