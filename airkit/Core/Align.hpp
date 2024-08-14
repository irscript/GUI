#ifndef __ALIGN_H__
#define __ALIGN_H__

#include <airkit/AIR_PCH.hpp>

namespace airkit
{

    template <typename T>
    inline T align_up(T value, T alignment)
    {
        return (value + alignment - 1) & ~(alignment - 1);
    }

    // 向下对齐
    template <typename T>
    inline T align_down(T value, T alignment)
    {
        return value & ~(alignment - 1);
    }
}
#endif // __ALIGN_H__