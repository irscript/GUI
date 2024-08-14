#ifndef __IRENDER_H__
#define __IRENDER_H__

#include <airkit/AIR_PCH.hpp>

namespace airkit
{
    enum class RenderAPI : uint32_t
    {
        Unknow,

        OpenGL,
    };

    struct IRender
    {
        virtual ~IRender() = 0;

        virtual RenderAPI getAPI() const = 0;

        virtual bool init(void *data = nullptr) = 0;
        virtual void shutdown() = 0;

        virtual void clear() = 0;
        virtual void clearColor(float r, float g, float b, float a) = 0;

        virtual void viewport(int x, int y, int width, int height) = 0;
    };
}

#endif // __IRENDER_H__