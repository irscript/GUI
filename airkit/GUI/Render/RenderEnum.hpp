#ifndef __RENDERENUM_H__
#define __RENDERENUM_H__

#include <airkit/AIR_PCH.hpp>
namespace airkit
{
    // 渲染API标识
    enum class RenderAPI : uint32_t
    {
        Unknow,

        OpenGL,
    };
    // 着色器阶段
    enum class ShaderStage : uint32_t
    {
        Unknow,
        Vertex,
        Fragment,
    };
}
#endif // __RENDERENUM_H__