#ifndef __UICOLOR_H__
#define __UICOLOR_H__
#include <airkit/AIR_PCH.hpp>
namespace airkit
{

    struct RGBA
    {
        union
        {
            uint32_t mRGBA;
            struct
            {
                uint8_t mR;
                uint8_t mG;
                uint8_t mB;
                uint8_t mA;
            };
        };
    };
}
#endif // __UICOLOR_H__